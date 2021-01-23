#include "asm_code.hh"
#include "remove_dead_code.hh"
#include "utils/utils.hh"

namespace latte::backend {

std::optional<register_type> get_modified_reg(const asm_code &c) {
	std::optional<register_type> reg;
	auto set_if_reg = [&](const asm_code::fst_param_type &par) {
		std::visit(overloaded {
			[&](const register_type &r) {
				reg = r;
			},
			[](auto &) {},
		}, par);
	};
	std::visit(overloaded {
		[&](const asm_code::mov &a) {
			set_if_reg(a.dst);
		},
		[&](const asm_code::binop &a) {
			reg = a.dst;
		},
		[&](const asm_code::neg &a) {
			set_if_reg(a.a);
		},
		[&](const asm_code::bool_not &a) {
			set_if_reg(a.a);
		},
		[&](const asm_code::pop &a) {
			reg = a.dst;
		},
		[&](const asm_code::call &) {
			reg = register_type::RAX;
		},
		[](auto &) {},
	}, c.val);
	return reg;
}

std::vector<register_type> get_used_regs(const asm_code &c, const std::string &epilog_label) {
	std::vector<register_type> regs;
	auto add_register_offset = [&](const register_offset &r) {
		regs.emplace_back(r.reg);
		if (r.index)
			regs.emplace_back(r.index->reg);
	};
	auto add_fst_param = [&](const asm_code::fst_param_type &par, bool modified) {
		std::visit(overloaded {
			[&](const register_type &r) {
				if (modified)
					regs.emplace_back(r);
			},
			[&](const register_offset &r) {
				add_register_offset(r);
			},
		}, par);
	};
	auto add_snd_param = [&](const asm_code::snd_param_type &par) {
		std::visit(overloaded {
			[&](const register_type &r) {
				regs.emplace_back(r);
			},
			[&](const register_offset &r) {
				add_register_offset(r);
			},
			[](auto &) {},
		}, par);
	};
	auto add_func_hdl = [&](const asm_code::func_hdl_type &f) {
		std::visit(overloaded {
			[&](const register_offset &r) {
				add_register_offset(r);
			},
			[](auto &) {},
		}, f);
	};
	std::visit(overloaded {
		[&](const asm_code::mov &a) {
			add_fst_param(a.dst, false);
			add_snd_param(a.src);
		},
		[&](const asm_code::binop &a) {
			if (a.op == core::exp::binary::arithmetic_op_type::DIV ||
					a.op == core::exp::binary::arithmetic_op_type::MOD) {
				regs.emplace_back(register_type::RAX);
				regs.emplace_back(register_type::RDX);
			} else {
				regs.emplace_back(a.dst);
			}
			add_snd_param(a.src);
		},
		[&](const asm_code::neg &a) {
			add_fst_param(a.a, true);
		},
		[&](const asm_code::bool_not &a) {
			add_fst_param(a.a, true);
		},
		[&](const asm_code::jump &a) {
			if (a.label == epilog_label)
				regs.emplace_back(register_type::RAX);
		},
		[&](const asm_code::cmp &a) {
			regs.emplace_back(a.a);
			add_snd_param(a.b);
		},
		[&](const asm_code::push &a) {
			add_snd_param(a.src);
		},
		[&](const asm_code::pop &a) {
			add_fst_param(a.dst, false);
		},
		[&](const asm_code::call &a) {
			add_func_hdl(a.func_hdl);
		},
		[&](const asm_code::ret &) {
			regs.emplace_back(register_type::RAX);
		},
		[](auto &) {},
	}, c.val);
	return regs;
}

void remove_dead_code(std::vector<asm_code> &code, const std::string &epilog_label) {
	std::set<register_type> alive_regs = {register_type::RSP};
	auto is_alive = [&](register_type reg) {
		return alive_regs.find(reg) != alive_regs.end();
	};
	auto add_alive = [&](const std::vector<register_type> &regs) {
		for (auto &reg : regs)
			alive_regs.emplace(reg);
	};
	std::vector<asm_code> rev_code;
	for (auto it = code.rbegin(); it != code.rend(); ++it) {
		auto mod = get_modified_reg(*it);
		bool added = false;
		if (std::holds_alternative<asm_code::call>(it->val) || !mod || is_alive(*mod)) {
			rev_code.emplace_back(*it);
			added = true;
		}
		if (mod)
			alive_regs.erase(*mod);
		if (added)
			add_alive(get_used_regs(*it, epilog_label));
	}
	code.clear();
	for (auto it = rev_code.rbegin(); it != rev_code.rend(); ++it)
		code.emplace_back(std::move(*it));
}

} // namespace latte::backend
