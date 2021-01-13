#include "asm_code.hh"

#include <cassert>
#include <ostream>

std::ostream &operator<<(std::ostream &ost, const latte::backend::register_type &r) {
	using register_type = latte::backend::register_type;
	switch (r) {
	case register_type::RAX: return ost << "rax";
	case register_type::RCX: return ost << "rcx";
	case register_type::RDX: return ost << "rdx";
	case register_type::RBX: return ost << "rbx";
	case register_type::RBP: return ost << "rbp";
	case register_type::RSP: return ost << "rsp";
	case register_type::RSI: return ost << "rsi";
	case register_type::RDI: return ost << "rdi";
	case register_type::R8: return ost << "r8";
	case register_type::R9: return ost << "r9";
	case register_type::R10: return ost << "r10";
	case register_type::R11: return ost << "r11";
	case register_type::R12: return ost << "r12";
	case register_type::R13: return ost << "r13";
	case register_type::R14: return ost << "r14";
	case register_type::R15: return ost << "r15";
	}
	assert(false);
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::register_offset &r) {
	ost << '[' << r.reg;
	if (r.offset > 0)
		ost << " + " << r.offset;
	else if (r.offset < 0)
		ost << " - " << -r.offset;
	return ost << ']';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::fixed_address &r) {
	return ost << r.addr_label;
}


std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code &a) {
	return ost << a.val;
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::mov &a) {
	return ost << "mov " << a.dst << ", " << a.src << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::binop &a) {
	using arithmetic_op_type = latte::core::exp::binary::arithmetic_op_type;
	switch (a.op) {
	case arithmetic_op_type::ADD:
		return ost << "add " << a.dst << ", " << a.src << '\n';
	case arithmetic_op_type::SUB:
		return ost << "sub " << a.dst << ", " << a.src << '\n';
	case arithmetic_op_type::MUL:
		return ost << "imul " << a.dst << ", " << a.src << '\n';
	case arithmetic_op_type::DIV:
		assert(a.dst == latte::backend::register_type::RAX);
		return ost << "idiv " << a.src << '\n';
	case arithmetic_op_type::MOD:
		assert(a.dst == latte::backend::register_type::RDX);
		return ost << "idiv " << a.src << '\n';
	}
	__builtin_unreachable();
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::neg &a) {
	return ost << "neg " << a.a << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::bool_not &a) {
	return ost << "xor " << a.a << ", 1\n";
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::jump &a) {
	return ost << "jmp " << a.label << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::cond_jump &a) {
	using rel_op_type = latte::core::exp::binary::rel_op_type;
	switch (a.op) {
	case rel_op_type::LTH:
		ost << "jl";
		break;
	case rel_op_type::LE:
		ost << "jle";
		break;
	case rel_op_type::GTH:
		ost << "jg";
		break;
	case rel_op_type::GE:
		ost << "jge";
		break;
	case rel_op_type::EQU:
		ost << "je";
		break;
	case rel_op_type::NE:
		ost << "jne";
		break;
	}
	return ost << " " << a.label << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::cmp &a) {
	return ost << "cmp " << a.a << ", " << a.b << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::push &a) {
	return ost << "push " << a.src << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::pop &a) {
	return ost << "pop " << a.dst << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::call &a) {
	return ost << "call " << a.func_name << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::label &a) {
	return ost << a.name << ":\n";
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::ret &) {
	return ost << "ret\n";
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::section &a) {
	return ost << "section " << a.name << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::alloc_string &a) {
	ost << a.name <<  " db ";
	for (char c : a.content) {
		if (isprint(c) && c != '\'')
			ost << '\'' << c << "', ";
		else
			ost << (int)c << ", ";
	}
	return ost << "0\n";
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::global_symbol &a) {
	return ost << "global " << a.name << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::extern_symbol &a) {
	return ost << "extern " << a.name << '\n';
}

std::ostream &operator<<(std::ostream &ost, const latte::backend::asm_code::syscall &) {
	return ost << "syscall\n";
}

std::ostream& operator<<(std::ostream& ost, const std::vector<latte::backend::asm_code> &v) {
	for (auto &i : v)
		ost << i;
	return ost;
}
