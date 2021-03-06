#pragma once

#include "core/tree_types.hh"

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

namespace latte::backend {

class ident_allocator {
public:
	enum class label_type {
		COND_NEXT,
		COND_TRUE,
		COND_FALSE,
		COND_END,
		LOOP_COND,
		LOOP_BODY,
		LOOP_END,
	};

	enum class var_type {
		EXP_CALL,
		EXP_BINARY,
		EXP_UNARY,
		VAR_ADDR_DEREF,
		OBJ_VTABLE,
		MEM_ADDR,
		ITER_POS,
	};

	struct user_var_data {
		core::ident id;
		core::type::val_type tp;
	};

private:
	std::unordered_map<label_type, size_t> _label_cnt;
	std::unordered_map<var_type, size_t> _var_cnt;

	std::string label_str(label_type type) {
		switch (type) {
		case label_type::COND_NEXT: return ".L_COND_NEXT";
		case label_type::COND_TRUE: return ".L_COND_TRUE";
		case label_type::COND_FALSE: return ".L_COND_FALSE";
		case label_type::COND_END: return ".L_COND_END";
		case label_type::LOOP_COND: return ".L_LOOP_COND";
		case label_type::LOOP_BODY: return ".L_LOOP_BODY";
		case label_type::LOOP_END: return ".L_LOOP_END";
		}
		__builtin_unreachable();
	}

	std::string tmp_var_str(var_type type) {
		switch (type) {
		case var_type::EXP_CALL: return ".res_exp_call";
		case var_type::EXP_BINARY: return ".res_exp_binary";
		case var_type::EXP_UNARY: return ".res_exp_unary";
		case var_type::VAR_ADDR_DEREF: return ".var_addr_deref";
		case var_type::OBJ_VTABLE: return ".obj_vtable";
		case var_type::MEM_ADDR: return ".mem_addr";
		case var_type::ITER_POS: return ".iter_pos";
		}
		__builtin_unreachable();
	}

	core::ident user_var_str(const core::ident &id, size_t num) {
		return concat(id, ".", num);
	}

	struct var_desc {
		struct internal_user_var_data {
			size_t id;
			core::type::val_type tp;
		};
		std::vector<internal_user_var_data> stack;
		size_t cur_id = 0;
	};

	std::vector<std::set<core::ident>> _block_vars;
	std::map<core::ident, var_desc> _vars_stack;

public:
	core::ident alloc_label(label_type type) {
		return concat(label_str(type), "_", std::to_string(_label_cnt[type]++));
	}

	core::ident alloc_tmp_var(var_type type) {
		return concat(tmp_var_str(type), "_", std::to_string(_var_cnt[type]++));
	}

	core::ident alloc_user_var(const core::ident &id, core::type::val_type tp) {
		bool success = _block_vars.back().emplace(id).second;
		assert(success);
		auto &var = _vars_stack[id];
		var.stack.emplace_back(var_desc::internal_user_var_data {var.cur_id++, tp});
		return user_var_str(id, var.stack.back().id);
	}

	std::optional<user_var_data> get_user_var(const core::ident &id) {
		auto it = _vars_stack.find(id);
		if (it == _vars_stack.end())
			return std::nullopt;
		assert(!it->second.stack.empty());
		return user_var_data {user_var_str(id, it->second.stack.back().id), it->second.stack.back().tp};
	}

	static core::ident get_method(const core::ident &class_id, const core::ident &method_id) {
		return concat(class_id, ".method.", method_id);
	}

	static core::ident get_vtable(const core::ident &class_id) {
		return concat(class_id, ".vtable");
	}

	static core::ident get_constructor(const core::ident &class_id) {
		return concat(class_id, ".constructor");
	}

	void push_block() {
		_block_vars.emplace_back();
	}

	void pop_block() {
		for (auto &id : _block_vars.back()) {
			auto it = _vars_stack.find(id);
			it->second.stack.pop_back();
		}
		_block_vars.pop_back();
	}
};

} // namespace latte::backend
