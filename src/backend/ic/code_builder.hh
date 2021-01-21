#pragma once

#include "code_types.hh"
#include "core/tree_types.hh"
#include "gen_ic.hh"
#include "ident_allocator.hh"

#include <memory>
#include <optional>
#include <vector>

namespace latte::backend {

class code_builder {
	std::vector<instr> _instrs;
	std::shared_ptr<ident_allocator> _id_alloc;
public:
	code_builder() : _id_alloc(std::make_shared<ident_allocator>()) {}
	code_builder(const code_builder &builder) : _id_alloc(builder._id_alloc) {}

	void push_instr(instr::instr_type i) { // TODO: use instr instead of instr_type
		bool add = visit(overloaded {
			[&](instr::label &i) {
				for (ssize_t idx = _instrs.size() - 1; idx >= 0; --idx) {
					auto &ci = _instrs[idx];
					if (!std::holds_alternative<instr::label>(ci.val)) {
						if (std::holds_alternative<instr::jump>(ci.val) &&
								std::get<instr::jump>(ci.val).label == i.name) {
							_instrs.erase(_instrs.begin() + idx);
						}
						break;
					}
				}
				return true;
			},
			[&](auto &) {
				return _instrs.empty() || (!std::holds_alternative<instr::ret>(_instrs.back().val) &&
					!std::holds_alternative<instr::jump>(_instrs.back().val));
			},
		}, i);
		if (add)
			_instrs.emplace_back(std::move(i));
	}
	void push_prepare_call_arg(instr::rvalue rv) {
		push_instr(instr::prepare_call_arg {
			.rv = std::move(rv),
		});
	}

	void push_call_ass(instr::lvalue lv, instr::func_addr func_hdl, size_t args_num) {
		push_instr(instr::call_ass {
			.lv = std::move(lv),
			.func_hdl = std::move(func_hdl),
			.args_num = args_num,
		});
	}

	void push_bin_ass(instr::lvalue lv, instr::rvalue rv1, instr::rvalue rv2, core::exp::binary::arithmetic_op_type op) {
		push_instr(instr::bin_ass {
			.lv = std::move(lv),
			.rv1 = std::move(rv1),
			.rv2 = std::move(rv2),
			.op = std::move(op),
		});
	}

	void push_unary_ass(instr::lvalue lv, core::exp::unary::op_type op, instr::rvalue rv) {
		push_instr(instr::unary_ass {
			.lv = std::move(lv),
			.op = std::move(op),
			.rv = std::move(rv),
		});
	}

	void push_set(instr::lvalue lv, instr::rvalue rv) {
		push_instr(instr::set {
			.lv = std::move(lv),
			.rv = std::move(rv),
		});
	}

	void push_jump(core::ident label) {
		push_instr(instr::jump {
			.label = std::move(label),
		});
	}

	void push_cond_jump(instr::rvalue rv1, instr::rvalue rv2, core::exp::binary::rel_op_type op, core::ident label_true) {
		push_instr(instr::cond_jump {
			.rv1 = std::move(rv1),
			.rv2 = std::move(rv2),
			.op = std::move(op),
			.label_true = std::move(label_true),
		});
	}

	void push_label(core::ident name) {
		push_instr(instr::label {
			.name = std::move(name),
		});
	}

	void push_ret(std::optional<instr::rvalue> val = std::nullopt) {
		push_instr(instr::ret {
			.val = std::move(val),
		});
	}

	void push_instrs(std::vector<instr> instrs) {
		for (auto &i : instrs)
			push_instr(std::move(i.val));
	}

	const std::vector<instr> &get_instrs() const {
		return _instrs;
	}

	ident_allocator &id_allocator() {
		return *_id_alloc;
	}
};

} // namespace latte::backend
