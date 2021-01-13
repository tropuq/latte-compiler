#pragma once

#include "asm_code.hh"
#include "core/tree_types.hh"

#include <map>
#include <string>

namespace latte::backend {

class const_str_allocator {
	std::map<std::string, fixed_address> _const_strs;
public:
	fixed_address get_str_addr(const std::string &s) {
		auto it = _const_strs.find(s);
		if (it == _const_strs.end()) {
			auto addr = fixed_address {"string." + std::to_string(_const_strs.size())};
			_const_strs.emplace(s, addr);
			return addr;
		}
		return it->second;
	}

	const std::map<std::string, fixed_address> &get_str_map() const {
		return _const_strs;
	}
};

}
