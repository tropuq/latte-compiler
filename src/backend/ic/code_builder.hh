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
	void push_prepare_call_arg(instr::arg a) {
		push_instr(instr::prepare_call_arg {
			.a = std::move(a),
		});
	}

	void push_call_ass(core::ident res_id, core::ident func_id, size_t args_num) {
		push_instr(instr::call_ass {
			.res_id = std::move(res_id),
			.func_id = std::move(func_id),
			.args_num = args_num,
		});
	}

	void push_bin_ass(core::ident id, instr::arg a1, instr::arg a2, core::exp::binary::arithmetic_op_type op) {
		push_instr(instr::bin_ass {
			.id = std::move(id),
			.a1 = std::move(a1),
			.a2 = std::move(a2),
			.op = std::move(op),
		});
	}

	void push_unary_ass(core::ident id, core::exp::unary::op_type op, instr::arg::var v) {
		push_instr(instr::unary_ass {
			.id = std::move(id),
			.op = std::move(op),
			.v = std::move(v),
		});
	}

	void push_set(core::ident id, instr::arg a) {
		push_instr(instr::set {
			.id = std::move(id),
			.a = std::move(a),
		});
	}

	void push_jump(std::string label) {
		push_instr(instr::jump {
			.label = std::move(label),
		});
	}

	void push_cond_jump(instr::arg a1, instr::arg a2, core::exp::binary::rel_op_type op, std::string label_true) {
		push_instr(instr::cond_jump {
			.a1 = std::move(a1),
			.a2 = std::move(a2),
			.op = std::move(op),
			.label_true = std::move(label_true),
		});
	}

	void push_label(std::string name) {
		push_instr(instr::label {
			.name = std::move(name),
		});
	}

	void push_ret(std::optional<instr::arg> a = std::nullopt) {
		push_instr(instr::ret {
			.a = std::move(a),
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
