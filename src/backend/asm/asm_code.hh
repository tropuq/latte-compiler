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
	int64_t offset;
	register_type reg;
};

struct fixed_address {
	std::string addr_label;
};

struct asm_code {
	using snd_param_type = std::variant<register_offset, register_type, int, bool, fixed_address>;
	struct mov {
		std::variant<register_offset, register_type> dst;
		snd_param_type src;
	};

	struct binop {
		core::exp::binary::arithmetic_op_type op;
		register_type dst;
		snd_param_type src;
	};

	struct neg {
		register_type a;
	};

	struct bool_not {
		register_type a;
	};

	struct jump {
		std::string label;
	};

	struct cond_jump {
		core::exp::binary::rel_op_type op;
		std::string label;
	};

	struct cmp {
		register_type a;
		snd_param_type b;
	};

	struct push {
		std::variant<register_type, int, bool> src;
	};

	struct pop {
		register_type dst;
	};

	struct call {
		core::ident func_name;
	};

	struct label {
		std::string name;
	};

	struct ret {};

	struct section {
		std::string name;
	};

	struct alloc_string {
		std::string name;
		std::string content;
	};

	struct global_symbol {
		std::string name;
	};

	struct extern_symbol {
		std::string name;
	};

	struct syscall {};

	using code_type = std::variant<mov, binop, neg, bool_not, jump, cond_jump,
		cmp, push, pop, call, label, ret, section, alloc_string,
		global_symbol, extern_symbol, syscall>;
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
std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::syscall &a);

std::ostream& operator<<(std::ostream& ost, const std::vector<latte::backend::asm_code> &v);
