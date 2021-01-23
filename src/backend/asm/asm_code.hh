#pragma once

#include "core/tree_types.hh"

#include <stdint.h>
#include <string>

namespace latte::backend {

enum class register_type {
	RAX,
	RCX,
	RDX,
	RBX,
	RBP,
	RSP,
	RSI,
	RDI,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
};

struct register_offset {
	struct index_data {
		register_type reg;
		size_t mul;
	};
	register_type reg;
	int64_t offset;
	std::optional<index_data> index;
};

struct fixed_address {
	std::string addr_label;
};

struct relative_address {
	std::string addr_label;
};

struct asm_code {
	using fst_param_type = std::variant<register_offset, register_type>;
	using snd_param_type = std::variant<register_offset, register_type, int, bool, fixed_address>;
	struct mov {
		fst_param_type dst;
		snd_param_type src;
	};

	struct binop {
		core::exp::binary::arithmetic_op_type op;
		register_type dst;
		snd_param_type src;
	};

	struct neg {
		fst_param_type a;
	};

	struct bool_not {
		fst_param_type a;
	};

	struct jump {
		core::ident label;
	};

	struct cond_jump {
		core::exp::binary::rel_op_type op;
		core::ident label;
	};

	struct cmp {
		register_type a;
		snd_param_type b;
	};

	struct push {
		snd_param_type src;
	};

	struct pop {
		register_type dst;
	};

	using func_hdl_type = std::variant<core::ident, register_offset>;
	struct call {
		func_hdl_type func_hdl;
	};

	struct label {
		core::ident name;
	};

	struct ret {};

	struct section {
		std::string name;
	};

	struct alloc_string {
		core::ident name;
		std::string content;
	};

	struct global_symbol {
		std::string name;
	};

	struct extern_symbol {
		std::string name;
	};

	struct define_vtable {
		core::ident name;
		std::vector<core::ident> methods;
	};

	using code_type = std::variant<mov, binop, neg, bool_not, jump, cond_jump,
		cmp, push, pop, call, label, ret, section, alloc_string,
		global_symbol, extern_symbol, define_vtable>;
	code_type val;
	asm_code(code_type v) : val(std::move(v)) {}
};

} // namespace latte::backend

std::ostream &operator<<(std::ostream &ost, const latte::backend::register_type &r);
std::ostream &operator<<(std::ostream &ost, const latte::backend::register_offset &r);
std::ostream &operator<<(std::ostream &ost, const latte::backend::fixed_address &r);

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::mov &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::binop &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::neg &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::bool_not &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::jump &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::cond_jump &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::cmp &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::push &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::pop &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::call &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::label &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::ret &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::section &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::alloc_string &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::global_symbol &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::extern_symbol &a);
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::define_vtable &a);

std::ostream& operator<<(std::ostream& ost, const std::vector<latte::backend::asm_code> &v);
