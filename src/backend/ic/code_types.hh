#pragma once

#include "core/tree_types.hh"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace latte::backend {

struct instr {
	struct variable_id {
		core::ident id;
		variable_id() {}
		variable_id(core::ident i) : id(std::move(i)) {}
	};
	struct address_offset {
		variable_id var;
		std::optional<variable_id> index;
		size_t offset;
	};
	struct null {};
	struct fixed_address {
		core::ident addr;
	};

	struct lvalue {
		using val_type = std::variant<variable_id, address_offset>;
		val_type val;
		core::type::val_type tp;
		lvalue() {}
		lvalue(val_type v, core::type::val_type t)
			: val(std::move(v)), tp(std::move(t)) {}
		lvalue(val_type v, core::type::single_type::val_type t)
			: val(std::move(v)), tp(core::type::single_type {std::move(t)}) {}
	};
	struct rvalue {
		using val_type = std::variant<variable_id, fixed_address,
			address_offset, int, bool, std::string, null>;
		val_type val;
		core::type::val_type tp;
		rvalue() {}
		rvalue(val_type v, core::type::val_type t)
			: val(std::move(v)), tp(std::move(t)) {}
		rvalue(val_type v, core::type::single_type::val_type t)
			: val(std::move(v)), tp(core::type::single_type {std::move(t)}) {}
	};

	struct func_addr {
		using val_type = std::variant<variable_id, address_offset>;
		val_type val;
		func_addr() {}
		func_addr(core::ident v) : val(std::move(v)) {}
		func_addr(variable_id v) : val(std::move(v)) {}
		func_addr(address_offset v) : val(std::move(v)) {}
	};

	struct prepare_call_arg {
		rvalue rv;
	};
	struct call_ass {
		lvalue lv;
		func_addr func_hdl;
		size_t args_num;
	};
	struct bin_ass {
		lvalue lv;
		rvalue rv1;
		rvalue rv2;
		core::exp::binary::arithmetic_op_type op;
	};
	struct unary_ass {
		lvalue lv;
		core::exp::unary::op_type op;
		rvalue rv;
	};
	struct set {
		lvalue lv;
		rvalue rv;
	};
	struct jump {
		core::ident label;
	};
	struct cond_jump {
		rvalue rv1;
		rvalue rv2;
		core::exp::binary::rel_op_type op;
		core::ident label_true;
	};
	struct label {
		core::ident name;
	};
	struct ret {
		std::optional<rvalue> val;
	};

	using instr_type = std::variant<prepare_call_arg, call_ass, bin_ass,
		unary_ass, set, jump, cond_jump, label, ret>;
	instr_type val;
	instr(instr_type instr) : val(std::move(instr)) {}
};

struct func {
	struct metadata {
		core::ident name;
		std::vector<core::ident> args;
	} meta;
	std::vector<instr> instrs;
};

struct vtable_description {
	core::ident vtable_addr_name;
	std::vector<core::ident> methods_addr_name;
};

struct program {
	std::vector<vtable_description> vtables;
	std::vector<func> funcs;
};

} // namespace latte::backend

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::variable_id &v) {
	return ost << "{" << v.id << "}";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::null &) {
	return ost << "null";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::address_offset &v) {
	ost << v.var << "[";
	if (v.index)
		ost << *v.index;
	if (v.index && v.offset)
		ost << "+";
	if (v.offset || !v.index)
		ost << v.offset;
	ost << "]";
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::fixed_address &v) {
	return ost << "[" << v.addr << "]";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::lvalue &v) {
	return ost << "(" << v.tp << ")" << v.val;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::rvalue &v) {
	return ost << "(" << v.tp << ")" << v.val;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::func_addr &v) {
	return ost << v.val;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::prepare_call_arg &i) {
	return ost << "prep " << i.rv;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::call_ass &i) {
	return ost << i.lv << " = " << i.func_hdl << "(args_num: " << i.args_num << ")";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::bin_ass &i) {
	return ost << i.lv << " = " << i.rv1 << " " << i.op << " " << i.rv2;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::unary_ass &i) {
	return ost << i.lv << " = " << i.op << " " << i.rv;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::set &i) {
	return ost << i.lv << " = " << i.rv;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::jump &i) {
	return ost << "goto " << i.label;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::cond_jump &i) {
	return ost << "if " << i.rv1 << " " << i.op << " " << i.rv2 << " goto " << i.label_true;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::label &i) {
	return ost << i.name << ":";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::ret &i) {
	ost << "return";
	if (i.val)
		ost << " " << *i.val;
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr &i) {
	return ost << i.val;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::func &f) {
	ost << f.meta.name << ":\n"; // TODO: print args
	for (auto &i : f.instrs)
		ost << "  " << i << '\n';
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::vtable_description &v) {
	return ost << v.vtable_addr_name << ": " << v.methods_addr_name << '\n';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::program &p) {
	bool first = true;
	for (auto &f : p.funcs) {
		if (!first) {
			ost << '\n';
		}
		ost << f;
		first = false;
	}
	return ost;
}
