#pragma once

#include "core/tree_types.hh"

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace latte::backend {

struct instr {
	struct arg {
		struct var {
			core::ident id;
		};
		using arg_type = std::variant<var, int, bool, std::string>;
		arg_type val;
		core::type::type_enum tp;
	};

	struct prepare_call_arg {
		arg a;
	};
	struct call_ass {
		core::ident res_id;
		core::ident func_id;
		size_t args_num;
	};
	struct bin_ass {
		core::ident id;
		arg a1;
		arg a2;
		core::exp::binary::arithmetic_op_type op;
	};
	struct unary_ass {
		core::ident id;
		core::exp::unary::op_type op;
		arg::var v;
	};
	struct set {
		core::ident id;
		arg a;
	};
	struct jump {
		std::string label;
	};
	struct cond_jump {
		arg a1;
		arg a2;
		core::exp::binary::rel_op_type op;
		std::string label_true;
	};
	struct label {
		std::string name;
	};
	struct ret {
		std::optional<arg> a;
	};
	using instr_type = std::variant<prepare_call_arg, call_ass, bin_ass, unary_ass, set, jump, cond_jump, label, ret>;
	instr_type val;
	instr(instr_type instr) : val(std::move(instr)) {}
};

struct func {
	struct metadata {
		std::string name;
		std::vector<core::ident> args;
	} meta;
	std::vector<instr> instrs;
};

struct program {
	std::vector<func> funcs;
};

} // namespace latte::backend

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::arg::var &v) {
	return ost << "{" << v.id << "}";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::arg &a) {
	return ost << a.val;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::prepare_call_arg &i) {
	return ost << "prep " << i.a;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::call_ass &i) {
	return ost << i.res_id << " = " << i.func_id << "(args_num: " << i.args_num << ")";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::bin_ass &i) {
	return ost << i.id << " = " << i.a1 << " " << i.op << " " << i.a2;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::unary_ass &i) {
	return ost << i.id << " = " << i.op << " " << i.v;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::set &i) {
	return ost << i.id << " = " << i.a;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::jump &i) {
	return ost << "goto " << i.label;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::cond_jump &i) {
	return ost << "if " << i.a1 << " " << i.op << " " << i.a2 << " goto " << i.label_true;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::label &i) {
	return ost << i.name << ":";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::instr::ret &i) {
	ost << "return";
	if (i.a)
		ost << " " << *i.a;
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
