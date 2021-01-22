#pragma once

#include "asm_code.hh"
#include "core/tree_types.hh"

#include <map>
#include <vector>

namespace latte::backend {

class stack_allocator {
	std::map<core::ident, register_offset> _var_mem_locs;
	size_t _local_var_cnt = 0;
public:
	stack_allocator(std::vector<core::ident> args) {
		for (size_t i = 0; i < args.size(); ++i) {
			auto reg_off = register_offset {
				.reg = register_type::RBP,
				.offset = (int64_t)(i + 2) * 8,
				.index = std::nullopt,
			};
			_var_mem_locs.emplace(args[i], reg_off);
		}
	}

	register_offset get_var_mem_loc(const core::ident &var) {
		auto it = _var_mem_locs.find(var);
		if (it != _var_mem_locs.end())
			return it->second;
		auto reg_off = register_offset {
			// TODO: use different sizes depending on var type
			.reg = register_type::RBP,
			.offset = (int64_t)(++_local_var_cnt) * -8,
			.index = std::nullopt,
		};
		_var_mem_locs.emplace(var, reg_off);
		return reg_off;
	}

	register_offset get_var_mem_loc(const core::ident &var) const {
		auto it = _var_mem_locs.find(var);
		if (it != _var_mem_locs.end())
			return it->second;
		throw std::runtime_error("variable not in memory");
	}

	size_t get_local_var_num() const {
		return _local_var_cnt;
	}
};

}
