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
#include <iostream>

namespace latte::backend {

// TODO: ignore exp stmt results

const auto void_type = core::type::single_type {core::type::single_type::type_enum::VOID};
const auto int_type = core::type::single_type {core::type::single_type::type_enum::INT};
const auto str_type = core::type::single_type {core::type::single_type::type_enum::STR};
const auto bool_type = core::type::single_type {core::type::single_type::type_enum::BOOL};
const auto null_type = core::type::single_type {core::type::single_type::null_type {}};
const core::ident self_id = "@self";
const core::ident strings_equal_id = "_strings_equal";
const core::ident alloc_id = "_alloc";
const core::ident add_strings_id = "_add_strings";

struct gen_ic_data {
	struct class_data {
		struct method_data {
			size_t offset;
			core::type::val_type type;
			core::ident virt_class;
			core::ident name;
		};
		struct variable_data {
			size_t offset;
			core::type::val_type type;
		};
		core::type::single_type::class_type type;
		std::map<core::ident, variable_data> var_data;
		std::map<core::ident, method_data> meth_data;
		const core::class_def *cls_def;
	};

	std::map<core::ident, class_data> cls_data;
	std::map<core::ident, core::type::val_type> func_type;
	class_data *cur_cls_data = nullptr;
};

instr::rvalue convert_to_quad_expr(const core::exp::uptr &ex, code_builder &builder, gen_ic_data &gen_data);

instr::rvalue convert_to_rvalue(instr::lvalue lv) {
	instr::rvalue rv;
	rv.tp = std::move(lv.tp);
	std::visit([&](auto &v) {
		rv.val = std::move(v);
	}, lv.val);
	return rv;
}

instr::lvalue push_strings_equal_call(instr::rvalue rv1, instr::rvalue rv2, code_builder &builder) {
	instr::lvalue lv = {
		builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_CALL),
		bool_type
	};
	builder.push_prepare_call_arg(std::move(rv2));
	builder.push_prepare_call_arg(std::move(rv1));
	builder.push_call_ass(lv, strings_equal_id, 2);
	return lv;
}


instr::lvalue push_alloc_call(instr::rvalue size, core::type::val_type t, code_builder &builder) {
	instr::lvalue lv = {builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::MEM_ADDR), std::move(t)};
	builder.push_prepare_call_arg(std::move(size));
	builder.push_call_ass(lv, alloc_id, 1);
	return lv;
}

void push_add_strings(const instr::lvalue &lv, instr::rvalue left_rv, instr::rvalue right_rv, code_builder &builder) {
	builder.push_prepare_call_arg(std::move(right_rv));
	builder.push_prepare_call_arg(std::move(left_rv));
	builder.push_call_ass(lv, add_strings_id, 2);
}

void convert_to_quad_cond_jump(const core::exp::uptr &ex, code_builder &builder, gen_ic_data &gen_data,
		const core::ident &label_true, const core::ident &label_false, bool negate = false) {
	using exp = core::exp;
	visit(overloaded {
		[&](exp::binary &e) {
			visit(overloaded {
				[&](exp::binary::rel_op_type &op) {
					auto rv1 = convert_to_quad_expr(e.e1, builder, gen_data);
					auto rv2 = convert_to_quad_expr(e.e2, builder, gen_data);
					if (rv1.tp == str_type) {
						auto lv = push_strings_equal_call(std::move(rv1), std::move(rv2), builder);
						builder.push_cond_jump(convert_to_rvalue(std::move(lv)),
							{true, bool_type},
							negate ? core::negate_oper(op) : op, label_true);
					} else {
						builder.push_cond_jump(std::move(rv1), std::move(rv2),
							negate ? core::negate_oper(op) : op, label_true);
					}
					builder.push_jump(label_false);
				},
				[&](exp::binary::bool_op_type &op) {
					using bool_op_type = exp::binary::bool_op_type;
					if (op == (negate ? bool_op_type::OR : bool_op_type::AND))
						return convert_to_quad_cond_jump(ex, builder, gen_data, label_false, label_true, !negate);
					std::string label_next = builder.id_allocator().alloc_label(
						ident_allocator::label_type::COND_NEXT);
					convert_to_quad_cond_jump(e.e1, builder, gen_data, label_true, label_next, negate);
					builder.push_label(move(label_next));
					convert_to_quad_cond_jump(e.e2, builder, gen_data, label_true, label_false, negate);
				},
				[&](auto &) {
					assert(false && "Invalid operation type in condition");
				},
			}, e.op);
		},
		[&](exp::unary &e) {
			assert(e.op == exp::unary::op_type::NOT);
			convert_to_quad_cond_jump(e.e, builder, gen_data, label_true, label_false, !negate);
		},
		[&](auto &) {
			builder.push_cond_jump(
				convert_to_quad_expr(ex, builder, gen_data),
				{!negate, bool_type},
				exp::binary::rel_op_type::EQU, label_true);
			builder.push_jump(label_false);
		},
	}, ex->val);
}

void convert_to_quad_cond(const core::exp::uptr &ex, code_builder &builder, gen_ic_data &gen_data,
		std::vector<instr> true_instrs, std::vector<instr> false_instrs) {
	auto label_true = builder.id_allocator().alloc_label(ident_allocator::label_type::COND_TRUE);
	auto label_false = builder.id_allocator().alloc_label(ident_allocator::label_type::COND_FALSE);
	auto label_end = builder.id_allocator().alloc_label(ident_allocator::label_type::COND_END);
	convert_to_quad_cond_jump(ex, builder, gen_data, label_true, label_false);

	builder.push_label(std::move(label_true));
	builder.push_instrs(std::move(true_instrs));
	builder.push_jump(label_end);

	builder.push_label(std::move(label_false));
	builder.push_instrs(std::move(false_instrs));

	builder.push_label(std::move(label_end));
}

void convert_call_args(const std::vector<core::exp::uptr> &args, code_builder &builder, gen_ic_data &gen_data) {
	std::vector<instr::rvalue> rvs;
	for (auto &p : args)
		rvs.emplace_back(convert_to_quad_expr(p, builder, gen_data));
	for (auto it = rvs.rbegin(); it != rvs.rend(); ++it)
		builder.push_prepare_call_arg(std::move(*it));
}

instr::variable_id load_vtable_to_var(const instr::variable_id &obj, code_builder &builder) {
	auto res_id = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::OBJ_VTABLE);
	builder.push_set({res_id, null_type}, {
			instr::address_offset {
				.var = obj,
				.index = std::nullopt,
				.offset = 0,
			}, null_type
		});
	return res_id;
}

instr::lvalue load_addr_to_var(const instr::lvalue &lv, code_builder &builder) {
	return std::visit(overloaded {
		[&](const instr::address_offset &) {
			auto addr = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::VAR_ADDR_DEREF);
			builder.push_set({addr, lv.tp}, convert_to_rvalue(lv));
			return instr::lvalue {std::move(addr), lv.tp};
		},
		[&](auto &) {
			return lv;
		},
	}, lv.val);
}

instr::rvalue get_array_size(const instr::lvalue &arr, code_builder &builder) {
	auto arr_var = load_addr_to_var(arr, builder);
	return {
			instr::address_offset {
				.var = std::get<instr::variable_id>(arr_var.val),
				.index = std::nullopt,
				.offset = 0,
			}, int_type
		};
}

instr::lvalue get_array_elem(const instr::lvalue &arr, const instr::rvalue &pos, code_builder &builder) {
	assert(std::holds_alternative<core::type::array_type>(arr.tp));
	auto arr_var = load_addr_to_var(arr, builder);
	auto prepare_index = [&builder](const instr::rvalue &rv) {
		std::pair<std::optional<instr::variable_id>, size_t> ret;
		std::visit(overloaded {
			[&](const instr::address_offset &) {
				// TODO: avoid copy pasting
				auto addr = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::VAR_ADDR_DEREF);
				builder.push_set({addr, rv.tp}, std::move(rv));
				ret = {std::move(addr), 0};
			},
			[&](const int &v) {
				ret = {std::nullopt, v};
			},
			[&](const instr::variable_id &v) {
				ret = {std::move(v), 0};
			},
			[&](auto &) {
				assert(false);
			},
		}, rv.val);
		return ret;
	};
	auto arr_pos = prepare_index(pos);
	return {
		instr::address_offset {
			.var = std::get<instr::variable_id>(arr_var.val),
			.index = arr_pos.first,
			.offset = 1 + arr_pos.second,
		}, std::get<core::type::array_type>(arr.tp).val
	};
}


instr::rvalue get_default_value(const core::type::val_type &tp) {
	using st = core::type::single_type;
	using at = core::type::array_type;
	instr::rvalue ret;
	ret.tp = tp;
	std::visit(overloaded {
		[&](const st &s) {
			std::visit(overloaded {
				[&](const st::type_enum &t) {
					switch (t) {
					case st::type_enum::INT:
						ret = {0, tp};
						return;
					case st::type_enum::STR:
						ret = {"", tp};
						return;
					case st::type_enum::BOOL:
						ret = {false, tp};
						return;
					case st::type_enum::VOID:
						assert(false && "no default type for void");
					}
					assert(false);
				},
				[&](const st::class_type &) {
					ret = {instr::null {}, null_type};
				},
				[&](const st::null_type &) {
					ret = {instr::null {}, tp};
				},
			}, s.val);
		},
		[&](const at &) {
			ret = {instr::null {}, null_type};
		},
	}, tp);
	return ret;
}

void gen_incr(const instr::lvalue &lv, code_builder &builder) {
	instr::lvalue tmp_lv = {builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_BINARY), int_type};
	builder.push_bin_ass(tmp_lv, convert_to_rvalue(lv), {1, int_type}, core::exp::binary::arithmetic_op_type::ADD);
	builder.push_set(lv, convert_to_rvalue(tmp_lv));
}

instr::lvalue convert_to_quad_nested_var(const core::nested_var &var, code_builder &builder, gen_ic_data &gen_data) {
	instr::lvalue lv;
	visit(overloaded {
		[&](const core::nested_var::self &) {
			assert(gen_data.cur_cls_data != nullptr);
			lv = {self_id, gen_data.cur_cls_data->type};
		},
		[&](const core::func_call &f) {
			// class method
			auto res_id = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_CALL);
			lv = {res_id, std::move(lv.tp)};
			if (gen_data.cur_cls_data != nullptr) {
				auto it = gen_data.cur_cls_data->meth_data.find(f.id);
				if (it != gen_data.cur_cls_data->meth_data.end()) {
					convert_call_args(f.params, builder, gen_data);
					builder.push_prepare_call_arg({instr::variable_id {self_id},
						gen_data.cur_cls_data->type});

					auto vtable = load_vtable_to_var(self_id, builder);
					lv.tp = it->second.type;
					instr::func_addr f_addr = {
						instr::address_offset {
							.var = std::move(vtable),
							.index = std::nullopt,
							.offset = it->second.offset
						}
					};
					auto args_num = f.params.size() + 1;
					builder.push_call_ass(lv, std::move(f_addr), args_num);
					return;
				}
			}
			// global function
			auto it = gen_data.func_type.find(f.id);
			assert(it != gen_data.func_type.end());
			convert_call_args(f.params, builder, gen_data);

			lv.tp = it->second;
			instr::func_addr f_addr = f.id;
			auto args_num = f.params.size();
			builder.push_call_ass(lv, std::move(f_addr), args_num);
		},
		[&](const core::nested_var::field_ident &f) {
			auto local_var = builder.id_allocator().get_user_var(f);
			if (!local_var) { // class field
				auto &var_data = gen_data.cur_cls_data->var_data[f];
				lv.val = instr::address_offset {
					.var = self_id,
					.index = std::nullopt,
					.offset = var_data.offset + 1
				};
				lv.tp = var_data.type;
				return;
			}
			lv = {local_var->id, local_var->tp};
		},
		[&](const core::new_val &v) {
			std::visit(overloaded {
				[&](const core::type::single_type::class_type &t) {
					auto obj_init = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_CALL);
					lv = push_alloc_call({(int)gen_data.cls_data[t.id].var_data.size(), int_type},
						core::type::single_type {t}, builder);
					builder.push_prepare_call_arg(convert_to_rvalue(lv));
					builder.push_call_ass({std::move(obj_init), void_type},
						ident_allocator::get_constructor(t.id), 1);
				},
				[&](const core::type::array_type &t) {
					auto arr_mem = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::MEM_ADDR);
					auto size = convert_to_quad_expr(*v.arr_size, builder, gen_data);
					lv = push_alloc_call(size, t, builder);
					builder.push_set({
							instr::address_offset {
								.var = std::get<instr::variable_id>(lv.val),
								.index = std::nullopt,
								.offset = 0
							}, size.tp
						}, size);

					instr::lvalue iter_pos_lv = {builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::ITER_POS),
						int_type};

					auto label_body = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_BODY);
					auto label_cond = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_COND);
					auto label_loop_end = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_END);

					builder.push_set(iter_pos_lv, {0, int_type});

					builder.push_jump(label_cond);
					builder.push_label(label_body);
					auto arr_elem_lv = get_array_elem(lv, convert_to_rvalue(iter_pos_lv), builder);
					builder.push_set(arr_elem_lv, get_default_value(t.val));

					// iter_pos++;
					gen_incr(iter_pos_lv, builder);

					builder.push_label(std::move(label_cond));
					builder.push_cond_jump(convert_to_rvalue(std::move(iter_pos_lv)), std::move(size),
						core::exp::binary::rel_op_type::LTH, label_body);
					builder.push_label(std::move(label_loop_end));
				}
			}, v.tp);
		},
	}, var.base.val);

	for (auto &field : var.fields) {
		std::visit(overloaded {
			[](const core::nested_var::self &) {},
			[&](const core::func_call &f) {
				assert(std::holds_alternative<core::type::single_type>(lv.tp));
				auto sing_tp = std::get<core::type::single_type>(lv.tp);
				assert(std::holds_alternative<core::type::single_type::class_type>(sing_tp.val));
				auto cls_id = std::get<core::type::single_type::class_type>(sing_tp.val).id;

				auto &cls_data = gen_data.cls_data[cls_id];
				auto &meth_data = cls_data.meth_data[f.id];

				lv = load_addr_to_var(lv, builder);

				convert_call_args(f.params, builder, gen_data);
				builder.push_prepare_call_arg(convert_to_rvalue(lv));

				lv.tp = meth_data.type;
				auto vtable = load_vtable_to_var(std::get<instr::variable_id>(lv.val), builder);
				instr::func_addr f_addr = {
					instr::address_offset {
						.var = std::move(vtable),
						.index = std::nullopt,
						.offset = meth_data.offset
					}
				};
				auto args_num = f.params.size() + 1;
				lv.val = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_CALL);
				builder.push_call_ass(lv, std::move(f_addr), args_num);
			},
			[&](const core::nested_var::field_ident &f) {
				if (std::holds_alternative<core::type::single_type>(lv.tp)) {
					auto sing_tp = std::get<core::type::single_type>(lv.tp);
					assert(std::holds_alternative<core::type::single_type::class_type>(sing_tp.val));
					auto cls_id = std::get<core::type::single_type::class_type>(sing_tp.val).id;

					auto &cls_data = gen_data.cls_data[cls_id];
					auto &var_data = cls_data.var_data[f];
					lv = load_addr_to_var(lv, builder);
					lv.val = instr::address_offset {
						.var = std::get<instr::variable_id>(lv.val),
						.index = std::nullopt,
						.offset = var_data.offset + 1
					};
					lv.tp = var_data.type;
				} else {
					lv = load_addr_to_var(lv, builder);
					lv = {instr::address_offset {
							.var = std::get<instr::variable_id>(lv.val),
							.index = std::nullopt,
							.offset = 0
						}, int_type};
				}
			},
			[&](const core::nested_var::array_pos &a) {
				auto pos = convert_to_quad_expr(a, builder, gen_data);
				lv = get_array_elem(lv, std::move(pos), builder);
			}
		}, field.val);
	}
	return lv;
}

instr::rvalue convert_to_quad_expr(const core::exp::uptr &ex, code_builder &builder, gen_ic_data &gen_data) {
	using exp = core::exp;
	return  visit(overloaded {
		[&](core::nested_var &e) -> instr::rvalue {
			return convert_to_rvalue(convert_to_quad_nested_var(e, builder, gen_data));
		},
		[&](exp::binary &e) -> instr::rvalue {
			instr::lvalue lv;
			auto bin_res = builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_BINARY);
			visit(overloaded {
				[&](exp::binary::arithmetic_op_type &op) {
					auto left_rv = convert_to_quad_expr(e.e1, builder, gen_data);
					auto right_rv = convert_to_quad_expr(e.e2, builder, gen_data);
					lv = {std::move(bin_res), left_rv.tp};
					if (lv.tp == str_type)
						push_add_strings(lv, std::move(left_rv), std::move(right_rv), builder);
					else
						builder.push_bin_ass(lv, std::move(left_rv), std::move(right_rv), op);
				},
				[&](auto &) {
					lv = {std::move(bin_res), bool_type};
					code_builder true_builder(builder), false_builder(builder);
					true_builder.push_set(lv, {true, bool_type});
					false_builder.push_set(lv, {false, bool_type});
					convert_to_quad_cond(ex, builder, gen_data, true_builder.get_instrs(), false_builder.get_instrs());
				},
			}, e.op);
			return convert_to_rvalue(std::move(lv));
		},
		[&](exp::unary &e) -> instr::rvalue {
			auto rv = convert_to_quad_expr(e.e, builder, gen_data);
			instr::lvalue lv = {builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_UNARY), rv.tp};
			builder.push_unary_ass(lv, e.op, std::move(rv));
			return convert_to_rvalue(lv);
		},
		[&](exp::null_casted &) -> instr::rvalue {
			return {instr::null {}, null_type};
		},
		[&](exp::null &) -> instr::rvalue {
			return {instr::null {}, null_type};
		},
		[&](int &e) -> instr::rvalue {
			return {e, int_type};
		},
		[&](bool &e) -> instr::rvalue {
			return {e, bool_type};
		},
		[&](std::string &e) -> instr::rvalue {
			return {e, str_type};
		}
	}, ex->val);
}

void convert_to_quad_stmt(const core::stmt::uptr &s, code_builder &builder, gen_ic_data &gen_data) {
	using stmt = core::stmt;
	visit(overloaded {
		[&](stmt::block &bl) {
			builder.id_allocator().push_block();
			for (auto &stmt : bl.bl->stmts)
				convert_to_quad_stmt(stmt, builder, gen_data);
			builder.id_allocator().pop_block();
		},
		[&](stmt::decl &s) {
			for (auto &item : s.items) {
				auto rv = item->init_val ?
					convert_to_quad_expr(*item->init_val, builder, gen_data) :
					get_default_value(s.tp.val);
				instr::lvalue lv = {builder.id_allocator().alloc_user_var(item->id, s.tp.val), s.tp.val};
				builder.push_set(std::move(lv), std::move(rv));
			}
		},
		[&](stmt::ass &s) {
			auto rv = convert_to_quad_expr(s.e, builder, gen_data);
			auto lv = convert_to_quad_nested_var(s.v, builder, gen_data);
			instr::lvalue tmp_lv = {builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_UNARY), lv.tp};
			builder.push_set(tmp_lv, std::move(rv));
			builder.push_set(std::move(lv), convert_to_rvalue(std::move(tmp_lv)));
		},
		[&](stmt::unary &s) {
			using arithmetic_op_type = core::exp::binary::arithmetic_op_type;
			auto lv = convert_to_quad_nested_var(s.v, builder, gen_data);
			instr::lvalue lv_tmp = {builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_BINARY), lv.tp};
			instr::rvalue rv = {1, int_type};
			switch (s.op) {
			case stmt::unary::op_type::INCR:
				builder.push_bin_ass(lv_tmp, convert_to_rvalue(lv), std::move(rv), arithmetic_op_type::ADD);
				break;
			case stmt::unary::op_type::DECR:
				builder.push_bin_ass(lv_tmp, convert_to_rvalue(lv), std::move(rv), arithmetic_op_type::SUB);
				break;
			}
			builder.push_set(lv, convert_to_rvalue(lv_tmp));
		},
		[&](stmt::ret &s) {
			if (s.e)
				builder.push_ret(convert_to_quad_expr(*s.e, builder, gen_data));
			else
				builder.push_ret();
		},
		[&](stmt::cond &s) {
			code_builder true_builder(builder), false_builder(builder);
			convert_to_quad_stmt(s.s_true, true_builder, gen_data);
			if (s.s_false)
				convert_to_quad_stmt(*s.s_false, false_builder, gen_data);
			convert_to_quad_cond(s.e, builder, gen_data, true_builder.get_instrs(), false_builder.get_instrs());
		},
		[&](stmt::loop_while &s) {
			auto label_body = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_BODY);
			auto label_cond = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_COND);
			auto label_loop_end = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_END);

			builder.push_jump(label_cond);
			builder.push_label(label_body);
			convert_to_quad_stmt(s.s, builder, gen_data);
			builder.push_label(std::move(label_cond));
			convert_to_quad_cond_jump(s.e, builder, gen_data, std::move(label_body), label_loop_end);
			builder.push_label(std::move(label_loop_end));
		},
		[&](stmt::loop_for &s) {
			builder.id_allocator().push_block();

			instr::lvalue iter_pos_lv = {
				builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::ITER_POS), int_type};
			instr::lvalue iter_elem_lv = {builder.id_allocator().alloc_user_var(s.id, s.tp), s.tp};

			auto label_body = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_BODY);
			auto label_cond = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_COND);
			auto label_loop_end = builder.id_allocator().alloc_label(ident_allocator::label_type::LOOP_END);

			auto arr = convert_to_quad_nested_var(s.v, builder, gen_data);
			auto arr_size = get_array_size(arr, builder);

			builder.push_set(iter_pos_lv, instr::rvalue {0, int_type});

			builder.push_jump(label_cond);
			builder.push_label(label_body);
			auto arr_elem_lv = get_array_elem(arr, convert_to_rvalue(iter_pos_lv), builder);
			builder.push_set(iter_elem_lv, convert_to_rvalue(std::move(arr_elem_lv)));
			convert_to_quad_stmt(s.s, builder, gen_data);

			// iter_pos++;
			gen_incr(iter_pos_lv, builder);

			builder.push_label(std::move(label_cond));
			builder.push_cond_jump(convert_to_rvalue(std::move(iter_pos_lv)), arr_size,
				core::exp::binary::rel_op_type::LTH, label_body);
			builder.push_label(std::move(label_loop_end));
			builder.id_allocator().pop_block();
		},
		[&](stmt::exp &s) {
			convert_to_quad_expr(s.e, builder, gen_data);
		},
		[&](stmt::empty &) {},
	}, s->val);
}

func convert_to_quad_func(const core::func::uptr &f, gen_ic_data &gen_data) {
	func qf;
	qf.meta.name = gen_data.cur_cls_data == nullptr ?
		f->id : ident_allocator::get_method(gen_data.cur_cls_data->cls_def->class_id, f->id);
	code_builder builder;
	builder.id_allocator().push_block();
	if (gen_data.cur_cls_data != nullptr)
		qf.meta.args.emplace_back(self_id);
	for (auto &a : f->args) {
		auto id = builder.id_allocator().alloc_user_var(a.id, a.tp.val);
		qf.meta.args.emplace_back(std::move(id));
	}
	for (auto &stmt : f->bl->stmts)
		convert_to_quad_stmt(stmt, builder, gen_data);
	builder.id_allocator().pop_block();
	qf.instrs = builder.get_instrs();
	return qf;
}

func generate_quad_constructor(gen_ic_data &gen_data) {
	assert(gen_data.cur_cls_data != nullptr);
	auto cls_def = gen_data.cur_cls_data->cls_def;
	instr::variable_id self = self_id;

	func qf;
	qf.meta.name = ident_allocator::get_constructor(cls_def->class_id);
	qf.meta.args.emplace_back(self.id);

	code_builder builder;
	builder.id_allocator().push_block();
	if (cls_def->extends_id) {
		builder.push_prepare_call_arg({self, gen_data.cls_data[*cls_def->extends_id].type});
		builder.push_call_ass({builder.id_allocator().alloc_tmp_var(ident_allocator::var_type::EXP_CALL),
				void_type}, ident_allocator::get_constructor(*cls_def->extends_id), 1);
	}
	builder.push_set({instr::address_offset {
				.var = self,
				.index = std::nullopt,
				.offset = 0
			}, null_type
		}, {instr::fixed_address {ident_allocator::get_vtable(cls_def->class_id)}, null_type});
	for (auto &def : cls_def->defs) {
		if (!std::holds_alternative<core::class_def::field>(def))
			continue;
		auto &field = std::get<core::class_def::field>(def);
		auto &field_data = gen_data.cur_cls_data->var_data[field.id];
		instr::lvalue lv = {instr::address_offset {
				.var = self,
				.index = std::nullopt,
				.offset = field_data.offset + 1
			}, field_data.type};
		builder.push_set(std::move(lv), get_default_value(field_data.type));
	}

	builder.push_ret();

	builder.id_allocator().pop_block();
	qf.instrs = builder.get_instrs();
	return qf;
}

std::vector<func> convert_to_quad_class(const core::class_def::uptr &cls, gen_ic_data &gen_data) {
	std::vector<func> methods;
	gen_data.cur_cls_data = &gen_data.cls_data[cls->class_id];
	methods.emplace_back(generate_quad_constructor(gen_data));
	for (auto &cls_def : cls->defs) {
		std::visit(overloaded {
			[&](const core::func::uptr &f) {
				methods.emplace_back(convert_to_quad_func(f, gen_data));
			},
			[&](const core::class_def::field &) {}
		}, cls_def);
	}
	gen_data.cur_cls_data = nullptr;
	return methods;
}

gen_ic_data prepare_gen_ic_data(const core::program::uptr &p) {
	gen_ic_data data;
	data.func_type["printString"] = void_type;
	data.func_type["printInt"] = void_type;
	data.func_type["readString"] = str_type;
	data.func_type["readInt"] = int_type;
	data.func_type["error"] = void_type;
	for (auto &def : p->defs) {
		std::visit(overloaded {
			[&](const core::func::uptr &f) {
				data.func_type[f->id] = f->tp.val;
			},
			[&](const core::class_def::uptr &c) {
				if (c->extends_id)
					data.cls_data[c->class_id] = data.cls_data[*c->extends_id];
				auto &cur_cls_data = data.cls_data[c->class_id];
				cur_cls_data.cls_def = c.get();
				cur_cls_data.type = core::type::single_type::class_type {c->class_id};
				for (auto &cls_def : c->defs) {
					std::visit(overloaded {
						[&](const core::class_def::field &f) {
							auto success = cur_cls_data.var_data.emplace(f.id,
								gen_ic_data::class_data::variable_data {
									.offset = cur_cls_data.var_data.size(),
									.type = f.tp.val
								}).second;
							assert(success);
						},
						[&](const core::func::uptr &f) {
							auto it = cur_cls_data.meth_data.find(f->id);
							if (it == cur_cls_data.meth_data.end()) {
								auto &new_meth = cur_cls_data.meth_data[f->id];
								new_meth.offset = cur_cls_data.meth_data.size() - 1;
								new_meth.virt_class = c->class_id;
								new_meth.type = f->tp.val;
								new_meth.name = f->id;
							} else {
								it->second.virt_class = c->class_id;
							}
						}
					}, cls_def);
				}
			}
		}, def);
	}
	return data;
}

std::vector<vtable_description> prepare_vtables(gen_ic_data &gen_data) {
	std::vector<vtable_description> vtables;
	for (auto &cls : gen_data.cls_data) {
		vtable_description vt;
		vt.vtable_addr_name = ident_allocator::get_vtable(cls.first);

		std::map<size_t, gen_ic_data::class_data::method_data*> meth_off_map;
		for (auto &meth : cls.second.meth_data)
			meth_off_map[meth.second.offset] = &meth.second;
		for (auto &meth : meth_off_map)
			vt.methods_addr_name.emplace_back(
				ident_allocator::get_method(meth.second->virt_class, meth.second->name));

		vtables.emplace_back(std::move(vt));
	}
	return vtables;
}

program convert_to_quad_prog(const core::program::uptr &p) {
	auto gen_data = prepare_gen_ic_data(p);
	program qp;
	qp.vtables = prepare_vtables(gen_data);
	for (auto &def : p->defs) {
		std::visit(overloaded {
			[&](const core::func::uptr &f) {
				qp.funcs.emplace_back(convert_to_quad_func(f, gen_data));
			},
			[&](const core::class_def::uptr &c) {
				auto methods = convert_to_quad_class(c, gen_data);
				for (auto &m : methods)
					qp.funcs.emplace_back(std::move(m));
			}
		}, def);
	}
	return qp;
}

} // namespace latte::backend
