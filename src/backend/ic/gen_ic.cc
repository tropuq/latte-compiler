#include "utils/utils.hh"
#include "code_types.hh"
#include "core/tree_types.hh"
#include "gen_ic.hh"
#include "ident_allocator.hh"
#include "code_builder.hh"

#include <optional>
#include <cassert>
#include <variant>
#include <unordered_map>
#include <memory>
#include <map>
#include <set>

namespace latte::backend {

// TODO: ignore exp stmt results

using func_info = std::map<core::ident, core::type::type_enum>;
func_info func_type;

instr::arg convert_to_quad_expr(const core::exp::uptr &e, code_builder &builder);

void convert_to_quad_cond_jump(const core::exp::uptr &ex, code_builder &builder,
		const core::ident &label_true, const core::ident &label_false, bool negate = false) {
	using exp = core::exp;
	visit(overloaded {
		[&](exp::binary &e) {
			visit(overloaded {
				[&](exp::binary::rel_op_type &op) {
					auto qe1 = convert_to_quad_expr(e.e1, builder);
					auto qe2 = convert_to_quad_expr(e.e2, builder);
					if (qe1.tp == core::type::type_enum::STR) {
						auto res = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_CALL);
						builder.push_prepare_call_arg(std::move(qe2));
						builder.push_prepare_call_arg(std::move(qe1));
						builder.push_call_ass(res, "_strings_equal", 2);
						builder.push_cond_jump({
								.val = instr::arg::var {std::move(res)},
								.tp = core::type::type_enum::BOOL
							}, {
								.val = true,
								.tp = core::type::type_enum::BOOL
							},
							negate ? core::negate_oper(op) : op, label_true);
					} else {
						builder.push_cond_jump(std::move(qe1), std::move(qe2),
							negate ? core::negate_oper(op) : op, label_true);
					}
					builder.push_jump(label_false);
				},
				[&](exp::binary::bool_op_type &op) {
					using bool_op_type = exp::binary::bool_op_type;
					if (op == (negate ? bool_op_type::OR : bool_op_type::AND))
						return convert_to_quad_cond_jump(ex, builder, label_false, label_true, !negate);
					std::string label_next = builder.id_allocator().alloc_label(
						ident_allocator::label_type::COND_NEXT);
					convert_to_quad_cond_jump(e.e1, builder, label_true, label_next, negate);
					builder.push_label(move(label_next));
					convert_to_quad_cond_jump(e.e2, builder, label_true, label_false, negate);
				},
				[&](auto &) {
					assert(false && "Invalid operation type in condition");
				},
			}, e.op);
		},
		[&](exp::unary &e) {
			assert(e.op == exp::unary::op_type::NOT);
			convert_to_quad_cond_jump(e.e, builder, label_true, label_false, !negate);
		},
		[&](auto &) {
			builder.push_cond_jump(
				convert_to_quad_expr(ex, builder),
				instr::arg {
					.val = !negate,
					.tp = core::type::type_enum::BOOL
				}, exp::binary::rel_op_type::EQU, label_true);
			builder.push_jump(label_false);
		},
	}, ex->val);
}

void convert_to_quad_cond(const core::exp::uptr &ex, code_builder &builder,
		std::vector<instr> true_instrs, std::vector<instr> false_instrs) {
	auto label_true = builder.id_allocator().alloc_label(ident_allocator::label_type::COND_TRUE);
	auto label_false = builder.id_allocator().alloc_label(ident_allocator::label_type::COND_FALSE);
	auto label_end = builder.id_allocator().alloc_label(ident_allocator::label_type::COND_END);
	convert_to_quad_cond_jump(ex, builder, label_true, label_false);

	builder.push_label(std::move(label_true));
	builder.push_instrs(std::move(true_instrs));
	builder.push_jump(label_end);

	builder.push_label(std::move(label_false));
	builder.push_instrs(std::move(false_instrs));

	builder.push_label(std::move(label_end));
}

instr::arg convert_to_quad_expr(const core::exp::uptr &ex, code_builder &builder) {
	using exp = core::exp;
	return  visit(overloaded {
		[&](exp::var &e) -> instr::arg {
			auto var_data = builder.id_allocator().get_user_var(e.id);
			return instr::arg {
				.val = instr::arg::var {std::move(var_data.id)},
				.tp = var_data.tp
			};
		},
		[&](exp::call &e) -> instr::arg {
			auto res = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_CALL);
			std::vector<instr::arg> a;
			for (auto &p : e.params)
				a.emplace_back(convert_to_quad_expr(p, builder));
			for (auto it = a.rbegin(); it != a.rend(); ++it)
				builder.push_prepare_call_arg(std::move(*it));
			builder.push_call_ass(res, e.id, e.params.size());
			return instr::arg {
				.val = instr::arg::var {std::move(res)},
				.tp = func_type[e.id],
			};
		},
		[&](exp::binary &e) -> instr::arg {
			auto res = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_BINARY);
			core::type::type_enum tp;
			visit(overloaded {
				[&](exp::binary::arithmetic_op_type &op) {
					auto qe1 = convert_to_quad_expr(e.e1, builder);
					auto qe2 = convert_to_quad_expr(e.e2, builder);
					tp = qe1.tp;
					if (tp == core::type::type_enum::STR) {
						builder.push_prepare_call_arg(std::move(qe2));
						builder.push_prepare_call_arg(std::move(qe1));
						builder.push_call_ass(res, "_add_strings", 2);
					} else {
						builder.push_bin_ass(res, std::move(qe1), std::move(qe2), op);
					}
				},
				[&](auto &) {
					code_builder true_builder(builder), false_builder(builder);
					true_builder.push_set(res, {
						.val = true,
						.tp = core::type::type_enum::BOOL
					});
					false_builder.push_set(res, {
						.val = false,
						.tp = core::type::type_enum::BOOL
					});
					convert_to_quad_cond(ex, builder, true_builder.get_instrs(), false_builder.get_instrs());
				},
			}, e.op);
			return instr::arg {
				.val = instr::arg::var {std::move(res)},
				.tp = tp
			};
		},
		[&](exp::unary &e) -> instr::arg {
			auto res = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_UNARY);
			auto qe = convert_to_quad_expr(e.e, builder);
			assert(std::holds_alternative<instr::arg::var>(qe.val));
			builder.push_unary_ass(res, e.op, std::move(std::get<instr::arg::var>(qe.val)));
			return instr::arg {
				.val = instr::arg::var {std::move(res)},
				.tp = qe.tp
			};
		},
		[&](int &e) -> instr::arg {
			return instr::arg {
				.val = e,
				.tp = core::type::type_enum::INT
			};
		},
		[&](bool &e) -> instr::arg {
			return instr::arg {
				.val = e,
				.tp = core::type::type_enum::BOOL
			};
		},
		[&](std::string &e) -> instr::arg {
			return instr::arg {
				.val = e,
				.tp = core::type::type_enum::STR
			};
		}
	}, ex->val);
}

instr::arg get_default_value(core::type::type_enum tp) {
	using types = core::type::type_enum;
	switch (tp) {
	case types::INT: return instr::arg {.val = 0, .tp = tp};
	case types::STR: return instr::arg {.val = "", .tp = tp};
	case types::BOOL: return instr::arg {.val = false, .tp = tp};
	case types::VOID: assert(false && "no default type for void");
	}
	__builtin_unreachable();
}

void convert_to_quad_stmt(const core::stmt::uptr &s, code_builder &builder) {
	using stmt = core::stmt;
	visit(overloaded {
		[&](stmt::block &bl) {
			builder.id_allocator().push_block();
			for (auto &stmt : bl.bl->stmts)
				convert_to_quad_stmt(stmt, builder);
			builder.id_allocator().pop_block();
		},
		[&](stmt::decl &s) {
			for (auto &item : s.items) {
				builder.push_set(builder.id_allocator().alloc_user_var(item->id, s.tp.val), item->init_val ?
					convert_to_quad_expr(*item->init_val, builder) :
					get_default_value(s.tp.val));
			}
		},
		[&](stmt::ass &s) {
			builder.push_set(builder.id_allocator().get_user_var(s.id).id, convert_to_quad_expr(s.e, builder));
		},
		[&](stmt::unary &s) {
			using arithmetic_op_type = core::exp::binary::arithmetic_op_type;
			auto id = builder.id_allocator().get_user_var(s.id).id;
			auto id_arg = instr::arg {
				.val = instr::arg::var {id},
				.tp = core::type::type_enum::INT
			};
			auto one_arg = instr::arg {
				.val = 1,
				.tp = core::type::type_enum::INT
			};
			switch (s.op) {
			case stmt::unary::op_type::INCR:
				builder.push_bin_ass(std::move(id), std::move(id_arg), std::move(one_arg), arithmetic_op_type::ADD);
				break;
			case stmt::unary::op_type::DECR:
				builder.push_bin_ass(std::move(id), std::move(id_arg), std::move(one_arg), arithmetic_op_type::SUB);
				break;
			}
		},
		[&](stmt::ret &s) {
			if (s.e)
				builder.push_ret(convert_to_quad_expr(*s.e, builder));
			else
				builder.push_ret();
		},
		[&](stmt::cond &s) {
			code_builder true_builder(builder), false_builder(builder);
			convert_to_quad_stmt(s.s_true, true_builder);
			if (s.s_false)
				convert_to_quad_stmt(*s.s_false, false_builder);
			convert_to_quad_cond(s.e, builder, true_builder.get_instrs(), false_builder.get_instrs());
		},
		[&](stmt::loop_while &s) {
			auto label_body = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_BODY);
			auto label_cond = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_COND);
			auto label_loop_end = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_END);

			builder.push_jump(label_cond);
			builder.push_label(label_body);
			convert_to_quad_stmt(s.s, builder);
			builder.push_label(std::move(label_cond));
			convert_to_quad_cond_jump(s.e, builder, label_body, label_loop_end);
			builder.push_label(std::move(label_loop_end));
		},
		[&](stmt::exp &s) {
			convert_to_quad_expr(s.e, builder);
		},
		[&](stmt::empty &) {},
	}, s->val);
}

func convert_to_quad_func(const core::func::uptr &f) {
	func qf;
	qf.meta.name = f->id;
	code_builder builder;
	builder.id_allocator().push_block();
	for (auto &a : f->args) {
		auto id = builder.id_allocator().alloc_user_var(a.id, a.tp.val);
		qf.meta.args.emplace_back(std::move(id));
	}
	for (auto &stmt : f->bl->stmts)
		convert_to_quad_stmt(stmt, builder);
	builder.id_allocator().pop_block();
	qf.instrs = builder.get_instrs();
	return qf;
}

program convert_to_quad_prog(const core::program::uptr &p) {
	program qp;
	func_type.clear();
	for (auto &f : p->funcs)
		func_type[f->id] = f->tp.val;
	for (auto &f : p->funcs)
		qp.funcs.emplace_back(convert_to_quad_func(f));
	return qp;
}

} // namespace latte::backend
