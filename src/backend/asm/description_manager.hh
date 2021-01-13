#pragma once

#include "asm_code.hh"
#include "const_string_allocator.hh"
#include "core/tree_types.hh"
#include "stack_allocator.hh"

#include <cassert>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

namespace latte::backend {

class description_manager {
	struct reg_desc {
		std::set<core::ident> vars;
	};
	struct var_desc {
		std::set<register_type> regs;
		bool in_mem;
	};

	std::unordered_map<register_type, reg_desc> _regs;
	std::set<register_type> _free_regs;
	std::map<core::ident, var_desc> _vars;
	std::set<register_type> _used_calle_save_regs;
	std::vector<asm_code> &_code;
	stack_allocator &_stack_alloc;
	const_str_allocator &_const_str_alloc;

	void add_calle_save_reg(register_type reg) {
		if (reg != register_type::RAX)
			_used_calle_save_regs.emplace(reg);
	}

	void extract_reg(register_type reg) {
		_free_regs.erase(reg);
		add_calle_save_reg(reg);
	}

	register_type extract_reg() {
		auto reg = _free_regs.extract(_free_regs.begin()).value();
		add_calle_save_reg(reg);
		return reg;
	}

	void kill_var_if_not_alive(const core::ident &id, const std::set<core::ident> &alive_after) {
		if (alive_after.find(id) == alive_after.end()) {
			auto &vd = _vars[id];
			for (auto &reg : vd.regs) {
				auto &rd = _regs[reg];
				bool success = rd.vars.erase(id);
				assert(success);
				if (rd.vars.empty()) {
					_regs.erase(reg);
					_free_regs.emplace(reg);
				}
			}
			_vars.erase(id);
		}
	}

	void assure_reg_can_be_discarded_no_alloc(register_type reg) {
		auto &rd = _regs[reg];
		for (auto &i : rd.vars) {
			auto &vd = _vars[i];
			assert(vd.regs.size() > 0);
			if (!vd.in_mem && vd.regs.size() == 1) {
				_code.emplace_back(asm_code::mov {_stack_alloc.get_var_mem_loc(i), reg});
				vd.in_mem = true;
			}
		}
	}

	void duplicate_reg_desc(register_type reg) {
		assert(!_free_regs.empty());
		auto dup_reg = extract_reg();
		_code.emplace_back(asm_code::mov {dup_reg, reg});
		_regs[dup_reg].vars = _regs[reg].vars;
		for (auto &v : _regs[reg].vars)
			_vars[v].regs.emplace(dup_reg);
	}

	void assure_reg_can_be_discarded_alloc(register_type reg) {
		auto &rd = _regs[reg];
		for (auto &i : rd.vars) {
			auto &vd = _vars[i];
			assert(vd.regs.size() > 0);
			if (!vd.in_mem && vd.regs.size() == 1) {
				duplicate_reg_desc(reg);
				break;
			}
		}
	}

	void assure_alive_reg_can_be_discarded(register_type reg) {
		auto reg_data_must_be_stored = [&](register_type reg) {
			for (auto &v : _regs[reg].vars)
				if (!_vars[v].in_mem && _vars[v].regs.size() <= 1)
					return true;
			return false;
		};
		if (reg_data_must_be_stored(reg)) {
			if (!_free_regs.empty())
				assure_reg_can_be_discarded_alloc(reg);
			else
				assure_reg_can_be_discarded_no_alloc(reg);
		}
	}

	void assure_reg_can_be_discarded(register_type reg, const std::set<core::ident> &alive_after) {
		auto reg_data_must_be_stored = [&](register_type reg, const std::set<core::ident> &alive_after) {
			for (auto &v : _regs[reg].vars)
				if (alive_after.find(v) != alive_after.end() && !_vars[v].in_mem && _vars[v].regs.size() <= 1)
					return true;
			return false;
		};
		if (reg_data_must_be_stored(reg, alive_after)) {
			if (!_free_regs.empty())
				assure_reg_can_be_discarded_alloc(reg);
			else
				assure_reg_can_be_discarded_no_alloc(reg);
		}
	}

	void check_reg_alive_opt_before_discard(register_type reg, const std::set<core::ident> &alive_after) {
		auto should_reg_be_duplicated = [&](register_type reg, const std::set<core::ident> &alive_after) {
			for (auto &v : _regs[reg].vars)
				if (alive_after.find(v) != alive_after.end() && _vars[v].regs.size() <= 1)
					return true;
			return false;
		};
		if (should_reg_be_duplicated(reg, alive_after) && !_free_regs.empty())
			duplicate_reg_desc(reg);
	}

	void discard_register_and_alloc(register_type reg) {
		auto &rd = _regs[reg];
		for (auto &v : rd.vars)
			_vars[v].regs.erase(reg);
		rd.vars.clear();
	}

public:
	description_manager(std::vector<asm_code> &code, stack_allocator &stack_alloc,
		const_str_allocator &const_str_alloc, const std::set<core::ident> &beg_alive)
		: _free_regs {register_type::RAX, register_type::RCX,
			register_type::RDX, register_type::RBX, register_type::RSI,
			register_type::RDI, register_type::R8, register_type::R9,
			register_type::R10, register_type::R11, register_type::R12,
			register_type::R13, register_type::R14, register_type::R15},
			_code(code), _stack_alloc(stack_alloc), _const_str_alloc(const_str_alloc) {
		// alive variables are in memory at the beggining
		for (auto &i : beg_alive)
			_vars[i].in_mem = true;
	}

	register_type get_free_reg() {
		if (_free_regs.empty()) {
			auto reg = register_type::RCX; // TODO: choose properly
			assure_alive_reg_can_be_discarded(reg);
			discard_register_and_alloc(reg);
			return reg;
		}
		auto reg = extract_reg();
		return reg;
	}

	void get_free_reg(register_type reg) {
		if (_free_regs.find(reg) == _free_regs.end()) {
			assure_alive_reg_can_be_discarded(reg);
			discard_register_and_alloc(reg);
		} else {
			extract_reg(reg);
		}
	}

	register_type get_free_reg_div() {
		if (_free_regs.empty()) {
			auto reg = register_type::RCX; // TODO: choose properly but don't choose RAX, RDX
			assure_alive_reg_can_be_discarded(reg);
			discard_register_and_alloc(reg);
			return reg;
		}
		auto reg = extract_reg(); // TODO: don't choose RAX, RDX
		return reg;
	}

	void free_reg(register_type reg) {
		discard_register_and_alloc(reg);
		_regs.erase(reg);
		_free_regs.emplace(reg);
	}

	void associate_reg_with_var(register_type reg, const core::ident &var) {
		_regs[reg].vars.emplace(var);
		_vars[var].regs.emplace(reg);
	}

	// Only this register contains the value of the variable.
	void associate_reg_with_modified_var(register_type reg, const core::ident &var) {
		auto &vd = _vars[var];
		auto &rd = _regs[reg];
		assert(rd.vars.empty());
		for (auto &reg : vd.regs)
			_regs[reg].vars.erase(var);
		vd.in_mem = false;
		vd.regs = {reg};
		rd.vars = {var};
	}

	register_type load_var_to_reg(const core::ident &var) {
		auto &vd = _vars[var];
		if (!vd.regs.empty())
			return *vd.regs.begin();
		assert(vd.in_mem);
		auto reg = get_free_reg();
		_code.emplace_back(asm_code::mov {reg, _stack_alloc.get_var_mem_loc(var)});
		associate_reg_with_var(reg, var);
		return reg;
	}

	void load_var_to_reg(const core::ident &var, register_type reg) {
		auto &vd = _vars[var];
		if (vd.regs.find(reg) != vd.regs.end())
			return;

		get_free_reg(reg);

		if (vd.regs.empty()) {
			assert(vd.in_mem);
			_code.emplace_back(asm_code::mov {reg, _stack_alloc.get_var_mem_loc(var)});
		} else {
			_code.emplace_back(asm_code::mov {reg, *vd.regs.begin()});
		}

		associate_reg_with_var(reg, var);
	}

	register_type load_var_to_reg_div(const core::ident &var) {
		auto &vd = _vars[var];
		if (!vd.regs.empty())
			return *vd.regs.begin();
		assert(vd.in_mem);
		auto reg = get_free_reg_div();
		_code.emplace_back(asm_code::mov {reg, _stack_alloc.get_var_mem_loc(var)});
		associate_reg_with_var(reg, var);
		return reg;
	}

	std::optional<register_type> try_load_var_to_reg(const core::ident &var) {
		auto &vd = _vars[var];
		if (!vd.regs.empty())
			return *vd.regs.begin();
		assert(vd.in_mem);
		if (_free_regs.empty())
			return std::nullopt;
		auto reg = extract_reg();
		_code.emplace_back(asm_code::mov {reg, _stack_alloc.get_var_mem_loc(var)});
		associate_reg_with_var(reg, var);
		return reg;
	}

	register_type load_string_to_reg(const std::string &s) {
		auto reg = get_free_reg();
		_code.emplace_back(asm_code::mov {reg, _const_str_alloc.get_str_addr(s)});
		return reg;
	}

	// Treat this register as if the content of it was erased. If needed, move the content
	// of it to memory or other registers. Update descriptions.
	void detach_reg(register_type reg, const std::set<core::ident> &alive_after) {
		assure_reg_can_be_discarded(reg, alive_after);
		check_reg_alive_opt_before_discard(reg, alive_after); // should check if it is used, not alive
		discard_register_and_alloc(reg);
	}

	void move_vars_to_memory(const std::set<core::ident> &vars) {
		for (auto &i : vars) {
			auto &vd = _vars[i];
			if (!vd.in_mem) {
				assert(!vd.regs.empty());
				_code.emplace_back(asm_code::mov {_stack_alloc.get_var_mem_loc(i), *vd.regs.begin()});
				vd.in_mem = true;
			}
		}
	}

	void kill_unused_vars(const std::set<core::ident> &alive_after) {
		for (auto vit = _vars.begin(); vit != _vars.end();)
			kill_var_if_not_alive((vit++)->first, alive_after);
	}

	asm_code::snd_param_type get_var_loc(const core::ident &id) {
		auto &vd = _vars[id];
		assert(vd.in_mem || !vd.regs.empty());
		if (!vd.regs.empty())
			return *vd.regs.begin();
		else
			return _stack_alloc.get_var_mem_loc(id);
	}

	const std::set<register_type> &get_used_callee_save_regs() const {
		return _used_calle_save_regs;
	}
};

}
