#pragma once

#include "code_pos.hh"
#include "utils/utils.hh"

#include <vector>
#include <variant>
#include <string>
#include <memory>
#include <optional>
#include <functional>

namespace latte::core {

struct tree_node {
	code_pos pos;
};

using ident = std::string;

struct type : tree_node {
	struct single_type {
		enum class type_enum {
			INT,
			STR,
			BOOL,
			VOID,
		};
		struct class_type {
			ident id;
		};
		using val_type = std::variant<type_enum, class_type>;
		val_type val;
	};
	struct array_type {
		struct single_type val;
	};
	using val_type = std::variant<single_type, array_type>;
	val_type val;
};

struct arg : tree_node {
	ident id;
	type tp;
};

struct block;
struct class_def;
struct exp;
struct func;
struct func_call;
struct item;
struct nested_var;
struct new_val;
struct program;
struct stmt;

struct program : tree_node {
	using uptr = std::unique_ptr<program>;
	using glob_def_type = std::variant<std::unique_ptr<func>, std::unique_ptr<class_def>>;
	std::vector<glob_def_type> defs;
};

struct class_def : tree_node {
	using uptr = std::unique_ptr<class_def>;
	struct field : tree_node {
		ident id;
		type tp;
	};
	using glob_def_type = std::variant<std::unique_ptr<func>, field>;
	ident class_id;
	std::optional<ident> extends_id;
	std::vector<glob_def_type> defs;
};

struct func : tree_node {
	using uptr = std::unique_ptr<func>;
	type tp;
	ident id;
	std::vector<arg> args;
	std::unique_ptr<block> bl;
};

struct block : tree_node {
	using uptr = std::unique_ptr<block>;
	std::vector<std::unique_ptr<stmt>> stmts;
};

struct func_call {
	core::ident id;
	std::vector<std::unique_ptr<core::exp>> params;
};

struct new_val {
	using tp_type = std::variant<type::single_type::class_type, type::array_type>;
	tp_type tp;
	std::optional<std::unique_ptr<core::exp>> arr_size; // if tp = array
};

struct nested_var {
	using field_ident = core::ident;
	struct self {};
	using base_type = std::variant<self, func_call, field_ident, new_val>;
	base_type base;

	using array_pos = std::unique_ptr<core::exp>;
	using field_type = std::variant<self, func_call, field_ident, array_pos>;
	std::vector<field_type> fields;
};

struct stmt : tree_node {
	using uptr = std::unique_ptr<stmt>;
	struct block {
		std::unique_ptr<core::block> bl;
	};
	struct decl {
		core::type tp;
		std::vector<std::unique_ptr<core::item>> items;
	};
	struct ass {
		core::nested_var v;
		std::unique_ptr<core::exp> e;
	};
	struct unary {
		core::nested_var v;
		enum class op_type {
			INCR,
			DECR,
		} op;
	};
	struct ret {
		std::optional<std::unique_ptr<core::exp>> e; // nullopt = return void
	};
	struct cond {
		std::unique_ptr<core::exp> e;
		std::unique_ptr<core::stmt> s_true;
		std::optional<std::unique_ptr<core::stmt>> s_false; // nullopt = no false block
	};
	struct loop_while {
		std::unique_ptr<core::exp> e;
		std::unique_ptr<core::stmt> s;
	};
	struct loop_for {
		type::single_type tp;
		core::ident id;
		nested_var v;
		std::unique_ptr<core::stmt> s;
	};
	struct exp {
		std::unique_ptr<core::exp> e;
	};
	struct empty {};
	using val_type = std::variant<block, decl, ass, unary, ret, cond,
		loop_while, loop_for, exp, empty>;
	val_type val;
};

struct item : tree_node {
	using uptr = std::unique_ptr<item>;
	core::ident id;
	std::optional<std::unique_ptr<core::exp>> init_val; // nullopt = no initial value
};

struct exp : tree_node {
	using uptr = std::unique_ptr<exp>;
	struct binary {
		std::unique_ptr<core::exp> e1;
		std::unique_ptr<core::exp> e2;
		enum class arithmetic_op_type {
			ADD,
			SUB,
			MUL,
			DIV,
			MOD,
		};
		enum class bool_op_type {
			AND,
			OR,
		};
		enum class rel_op_type {
			LTH,
			LE,
			GTH,
			GE,
			EQU,
			NE,
		};
		using op_type = std::variant<arithmetic_op_type, bool_op_type, rel_op_type>;
		op_type op;
	};
	struct unary {
		std::unique_ptr<core::exp> e;
		enum class op_type {
			NOT,
			NEG,
		} op;
	};
	struct null {
		using tp_type = std::variant<type::single_type::class_type, type::array_type>;
		tp_type tp;
	};
	std::variant<nested_var, binary, unary, null, int, bool, std::string> val;
};

inline bool is_bool_oper(exp::binary::op_type op) {
	return std::holds_alternative<exp::binary::bool_op_type>(op);
}
inline bool is_arithmetic_oper(exp::binary::op_type op) {
	return std::holds_alternative<exp::binary::arithmetic_op_type>(op);
}
inline bool is_rel_oper(exp::binary::op_type op) {
	return std::holds_alternative<exp::binary::rel_op_type>(op);
}

template<typename T>
bool is_same_oper(const exp::binary::op_type &bin_op, T cmp_op) {
	if (std::holds_alternative<T>(bin_op))
		return std::get<T>(bin_op) == cmp_op;
	return false;
}

template<typename T>
std::function<T(T,T)> get_oper_func(exp::binary::arithmetic_op_type op) {
	using ar_op = exp::binary::arithmetic_op_type;
	switch (op) {
	case ar_op::ADD: return std::plus<T>();
	case ar_op::SUB: return std::minus<T>();
	case ar_op::MUL: return std::multiplies<T>();
	case ar_op::DIV: return std::divides<T>();
	case ar_op::MOD: return std::modulus<T>();
	}
	__builtin_unreachable();
}

template<typename RET, typename ARG>
struct do_nothing {
	constexpr RET operator()(const ARG &, const ARG &) const {
		return RET();
	}
};

template<>
inline std::function<std::string(std::string,std::string)> get_oper_func(exp::binary::arithmetic_op_type op) {
	using ar_op = exp::binary::arithmetic_op_type;
	switch (op) {
	case ar_op::ADD: return std::plus<std::string>();
	default: return do_nothing<std::string, std::string>();
	}
	__builtin_unreachable();
}

template<typename T>
std::function<bool(T,T)> get_oper_func(exp::binary::bool_op_type op) {
	using bool_op = exp::binary::bool_op_type;
	switch (op) {
	case bool_op::AND: return std::logical_or<T>();
	case bool_op::OR: return std::logical_and<T>();
	}
	__builtin_unreachable();
}

template<>
inline std::function<bool(std::string,std::string)> get_oper_func(exp::binary::bool_op_type) {
	return do_nothing<bool, std::string>();
}

template<typename T>
std::function<bool(T,T)> get_oper_func(exp::binary::rel_op_type op) {
	using rel_op = exp::binary::rel_op_type;
	switch (op) {
	case rel_op::LTH: return std::less<T>();
	case rel_op::LE: return std::less_equal<T>();
	case rel_op::GTH: return std::greater<T>();
	case rel_op::GE: return std::greater_equal<T>();
	case rel_op::EQU: return std::equal_to<T>();
	case rel_op::NE: return std::not_equal_to<T>();
	}
	__builtin_unreachable();
}

inline exp::binary::rel_op_type negate_oper(exp::binary::rel_op_type op) {
	using rel_op = exp::binary::rel_op_type;
	switch (op) {
	case rel_op::LTH: return rel_op::GE;
	case rel_op::LE: return rel_op::GTH;
	case rel_op::GTH: return rel_op::LE;
	case rel_op::GE: return rel_op::LTH;
	case rel_op::EQU: return rel_op::NE;
	case rel_op::NE: return rel_op::EQU;
	}
	__builtin_unreachable();
}

inline exp::binary::bool_op_type negate_oper(exp::binary::bool_op_type op) {
	using bool_op = exp::binary::bool_op_type;
	switch (op) {
	case bool_op::AND: return bool_op::OR;
	case bool_op::OR: return bool_op::AND;
	}
	__builtin_unreachable();
}

} // namespace latte::core

inline std::ostream& operator<<(std::ostream& ost, latte::core::type::single_type::type_enum tp) {
	using type_enum = latte::core::type::single_type::type_enum;
	switch (tp) {
	case type_enum::INT: return ost << "int";
	case type_enum::STR: return ost << "string";
	case type_enum::BOOL: return ost << "boolean";
	case type_enum::VOID: return ost << "void";
	}
	__builtin_unreachable();
}

inline std::ostream& operator<<(std::ostream& ost, latte::core::type::single_type::class_type tp) {
	return ost << "<" << tp.id << ">";

}

inline std::ostream& operator<<(std::ostream& ost, latte::core::type::single_type tp) {
	return ost << tp.val;
}

inline std::ostream& operator<<(std::ostream& ost, latte::core::type::array_type tp) {
	return ost << tp.val.val << "[]";
}

inline std::ostream& operator<<(std::ostream& ost, const latte::core::type &tp) {
	return ost << tp.val;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::binary::arithmetic_op_type &op) {
	using arithmetic_op_type = latte::core::exp::binary::arithmetic_op_type;
	switch (op) {
	case arithmetic_op_type::ADD: return ost << "+";
	case arithmetic_op_type::SUB: return ost << "-";
	case arithmetic_op_type::MUL: return ost << "*";
	case arithmetic_op_type::DIV: return ost << "/";
	case arithmetic_op_type::MOD: return ost << "%";
	}
	__builtin_unreachable();
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::binary::bool_op_type &op) {
	using bool_op_type = latte::core::exp::binary::bool_op_type;
	switch (op) {
	case bool_op_type::AND: return ost << "&&";
	case bool_op_type::OR: return ost << "||";
	}
	__builtin_unreachable();
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::binary::rel_op_type &op) {
	using rel_op_type = latte::core::exp::binary::rel_op_type;
	switch (op) {
	case rel_op_type::LTH: return ost << "<";
	case rel_op_type::LE: return ost << "<=";
	case rel_op_type::GTH: return ost << ">";
	case rel_op_type::GE: return ost << ">=";
	case rel_op_type::EQU: return ost << "==";
	case rel_op_type::NE: return ost << "!=";
	}
	__builtin_unreachable();
}

inline std::ostream &operator<<(std::ostream &ost, const enum latte::core::exp::unary::op_type &op) {
	using unary_op_type = latte::core::exp::unary::op_type;
	switch (op) {
	case unary_op_type::NOT: return ost << "!";
	case unary_op_type::NEG: return ost << "-";
	}
	__builtin_unreachable();
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::block::uptr &bl);
inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::uptr &st);
inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::uptr &e);

inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::null &e) {
	return ost << e.tp << " null";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::binary &e) {
	return ost << '(' << e.e1 << ' ' << e.op << ' ' << e.e2 << ')';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::unary &e) {
	return ost << e.op << e.e;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::exp::uptr &e) {
	return ost << e->val;
}


inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::block &st) {
	return ost << st.bl;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::item::uptr &it) {
	ost << it->id;
	if (it->init_val)
		ost << " = " << *it->init_val;
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::func_call &call) {
	return ost << call.id << '(' << call.params << ')';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::new_val &v) {
	ost << "new ";
	std::visit(overloaded {
		[&](const latte::core::type::array_type &tp) {
			ost << tp.val;
		},
		[&](auto &tp) {
			ost << tp;
		}
	}, v.tp);
	if (v.arr_size)
		ost << '[' << *v.arr_size << ']';
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::nested_var::self &) {
	return ost << "self";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::nested_var &v) {
	if (!v.fields.empty() && std::holds_alternative<latte::core::new_val>(v.base))
		ost << '(' << v.base << ')';
	else
		ost << v.base;
	for (auto &f : v.fields) {
		if (std::holds_alternative<latte::core::nested_var::array_pos>(f))
			ost << '[' << f << ']';
		else
			ost << '.' << f ;
	}
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::decl &st) {
	return ost << st.tp << ' ' << st.items << '\n';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::ass &st) {
	return ost << st.v << " = " << st.e << '\n';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::unary &st) {
	ost << st.v;
	using unary_op_type = latte::core::stmt::unary::op_type;
	switch (st.op) {
	case unary_op_type::INCR:
		ost << "++";
		break;
	case unary_op_type::DECR:
		ost << "--";
		break;
	}
	return ost << '\n';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::ret &st) {
	ost << "return";
	if (st.e)
		ost << ' ' << *st.e;
	return ost << '\n';
}

namespace {

inline std::ostream &_add_spaces_if_not_block(std::ostream &ost, const latte::core::stmt::uptr &st) {
	if (std::holds_alternative<latte::core::stmt::block>(st->val))
		ost << st->val;
	else
		ost << "  " << st->val;
	return ost;
}

} // namespace

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::cond &st) {
	ost << "if (" << st.e << ")\n";
	_add_spaces_if_not_block(ost, st.s_true);
	if (st.s_false) {
		ost << "else\n";
		_add_spaces_if_not_block(ost, *st.s_false);
	}
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::loop_while &st) {
	ost << "while (" << st.e << ")\n";
	_add_spaces_if_not_block(ost, st.s);
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::loop_for &st) {
	ost << "for (" << st.tp << ' ' << st.id << " : " << st.v << ")\n";
	_add_spaces_if_not_block(ost, st.s);
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::exp &st) {
	return ost << st.e << '\n';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::empty &) {
	return ost << ";" << '\n';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::stmt::uptr &st) {
	return ost << st->val;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::block::uptr &bl) {
	ost << "{\n";
	for (auto &stmt : bl->stmts) {
		std::stringstream ss;
		ss << stmt;
		ost << add_new_line_spaces(ss.str(), 2);
	}
	ost << "}\n";
	return ost;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::arg &a) {
	return ost << a.tp << ' ' << a.id;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::class_def::field &a) {
	return ost << a.tp << ' ' << a.id << '\n';
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::func::uptr &f) {
	return ost << f->tp << ' ' << f->id << '(' << f->args << ")\n" << f->bl;
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::class_def::uptr &c) {
	ost << "class " << c->class_id << ' ';
	if (c->extends_id)
		ost << "extends " << *c->extends_id << ' ';
	ost << "{\n";
	for (auto &d : c->defs) {
		std::stringstream ss;
		ss << d;
		ost << add_new_line_spaces(ss.str(), 2);
	}
	return ost << "}\n";
}

inline std::ostream &operator<<(std::ostream &ost, const latte::core::program::uptr &prog) {
	for (auto &def : prog->defs)
		ost << def;
	return ost;
}
