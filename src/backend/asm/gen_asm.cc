#include "backend/ic/control_flow_graph.hh"
#include "core/tree_types.hh"
#include "asm_code.hh"
#include "stack_allocator.hh"
#include "const_string_allocator.hh"
#include "description_manager.hh"

#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <iostream>

namespace latte::backend {

std::string gen_epilog_label(std::string name) {
	return std::move(name) + ".epilog";
}

class basic_block_code_generator {
	std::vector<asm_code> _code;

	const_str_allocator &_const_str_alloc;
	description_manager _desc_mgmt;

	std::string _epilog_label;

	bool _epilog_generated = false;

	void prepare_locs_for_bin_op(const instr::arg &a1, const instr::arg &a2, register_type &dst,
		asm_code::snd_param_type &src, const std::set<core::ident> &alive_after) {

		std::visit(overloaded {
			[&](instr::arg::var v) {
				dst = _desc_mgmt.load_var_to_reg(v.id);
			},
			[&](int v) {
				dst = _desc_mgmt.get_free_reg();
				_code.emplace_back(asm_code::mov {dst, v});
			},
			[&](bool v) {
				dst = _desc_mgmt.get_free_reg();
				_code.emplace_back(asm_code::mov {dst, v});
			},
			[&](auto) {
				assert(false);
			}
		}, a1.val);

		std::visit(overloaded {
			[&](instr::arg::var v) {
				if (alive_after.find(v.id) != alive_after.end())
					_desc_mgmt.try_load_var_to_reg(v.id);
				src = _desc_mgmt.get_var_loc(v.id);
			},
			[&](int v) {
				src = v;
			},
			[&](bool v) {
				src = v;
			},
			[&](auto) {
				assert(false);
			}
		}, a2.val);
	}

	void perform_div_op(const instr::arg &a1, const instr::arg &a2, core::exp::binary::arithmetic_op_type op,
		const core::ident &res, const std::set<core::ident> &alive_after) {

		auto rdx = register_type::RDX;
		_desc_mgmt.get_free_reg(rdx);

		auto rax = register_type::RAX;
		std::visit(overloaded {
			[&](instr::arg::var v) {
				_desc_mgmt.load_var_to_reg(v.id, rax);
			},
			[&](int v) {
				_desc_mgmt.get_free_reg(rax);
				_code.emplace_back(asm_code::mov {rax, v});
			},
			[&](auto) {
				assert(false);
			}
		}, a1.val);

		register_type src;
		std::visit(overloaded {
			[&](instr::arg::var v) {
				src = _desc_mgmt.load_var_to_reg_div(v.id);
			},
			[&](int v) {
				src = _desc_mgmt.get_free_reg_div();
				_code.emplace_back(asm_code::mov {src, v});
			},
			[&](auto) {
				assert(false);
			}
		}, a2.val);

		_desc_mgmt.detach_reg(rax, alive_after);

		register_type dst, free;
		if (op == core::exp::binary::arithmetic_op_type::MOD) {
			dst = rdx;
			free = rax;
		} else {
			dst = rax;
			free = rdx;
		}

		_code.emplace_back(asm_code::mov {rdx, 0});
		_code.emplace_back(asm_code::binop {op, dst, src});

		_desc_mgmt.associate_reg_with_modified_var(dst, res);
		_desc_mgmt.free_reg(free);
	}

	void gen_basic_block_epilog(const basic_block &b) {
		_epilog_generated = true;
		return _desc_mgmt.move_vars_to_memory(b.get_end_alive());
	}

public:
	basic_block_code_generator(const basic_block &b, stack_allocator &stack_alloc,
			const_str_allocator &const_str_alloc, std::string epilog_label)
			: _const_str_alloc(const_str_alloc), _desc_mgmt(_code, stack_alloc, _const_str_alloc, b.get_beg_alive()),
			_epilog_label(std::move(epilog_label)) {
		for (auto &i : b.get_instr_info()) {
			std::visit(overloaded {
				[&](instr::prepare_call_arg e) {
					std::visit(overloaded {
						[&](instr::arg::var v) {
							auto reg = _desc_mgmt.load_var_to_reg(v.id);
							_code.emplace_back(asm_code::push {reg});
						},
						[&](std::string v) {
							auto reg = _desc_mgmt.load_string_to_reg(v);
							_code.emplace_back(asm_code::push {reg});
						},
						[&](auto v) {
							_code.emplace_back(asm_code::push {v});
						},
					}, e.a.val);
				},
				[&](instr::call_ass e) {
					auto rax = register_type::RAX;
					_desc_mgmt.get_free_reg(rax);
					_code.emplace_back(asm_code::call {e.func_id});
					_desc_mgmt.associate_reg_with_modified_var(rax, e.res_id);

					// remove call arguments
					if (e.args_num > 0) {
						_code.emplace_back(
							asm_code::binop {
								core::exp::binary::arithmetic_op_type::ADD,
								register_type::RSP,
								(int)e.args_num * 8
							});
					}
				},
				[&](instr::bin_ass e) {
					// Because div/mod operations are different.
					if (e.op == core::exp::binary::arithmetic_op_type::MOD ||
							e.op == core::exp::binary::arithmetic_op_type::DIV) {
						perform_div_op(e.a1, e.a2, e.op, e.id, i.alive_after);
						return;
					}

					register_type dst;
					asm_code::snd_param_type src;
					prepare_locs_for_bin_op(e.a1, e.a2, dst, src, i.alive_after);

					_desc_mgmt.detach_reg(dst, i.alive_after);
					_code.emplace_back(asm_code::binop {e.op, dst, src});
					_desc_mgmt.associate_reg_with_modified_var(dst, e.id);
				},
				[&](instr::unary_ass e) {
					auto reg = _desc_mgmt.load_var_to_reg(e.v.id);
					_desc_mgmt.detach_reg(reg, i.alive_after);
					switch (e.op) {
					case core::exp::unary::op_type::NEG:
						_code.emplace_back(asm_code::neg {reg});
						break;
					case core::exp::unary::op_type::NOT:
						_code.emplace_back(asm_code::bool_not {reg});
						break;
					}
					_desc_mgmt.associate_reg_with_modified_var(reg, e.id);
				},
				[&](instr::set e) {
					std::visit(overloaded {
						[&](instr::arg::var v) {
							// load to register of copy existing register
							auto reg = _desc_mgmt.load_var_to_reg(v.id);
							_desc_mgmt.associate_reg_with_var(reg, e.id);
						},
						[&](std::string v) {
							auto reg = _desc_mgmt.load_string_to_reg(v);
							_desc_mgmt.associate_reg_with_var(reg, e.id);
						},
						[&](auto v) {
							auto reg = _desc_mgmt.get_free_reg();
							_code.emplace_back(asm_code::mov {reg, v});
							_desc_mgmt.associate_reg_with_var(reg, e.id);
						},
					}, e.a.val);
				},
				[&](instr::jump e) {
					gen_basic_block_epilog(b);
					_code.emplace_back(asm_code::jump {e.label});
				},
				[&](instr::cond_jump e) {
					register_type l;
					asm_code::snd_param_type r;
					prepare_locs_for_bin_op(e.a1, e.a2, l, r, i.alive_after);

					_code.emplace_back(asm_code::cmp {l, r});

					gen_basic_block_epilog(b);

					_code.emplace_back(asm_code::cond_jump {e.op, e.label_true});
				},
				[&](instr::label e) {
					_code.emplace_back(asm_code::label {e.name});
				},
				[&](instr::ret e) {
					gen_basic_block_epilog(b);
					if (e.a) {
						auto rax = register_type::RAX;
						std::visit(overloaded {
							[&](instr::arg::var v) {
								_desc_mgmt.load_var_to_reg(v.id, rax);
							},
							[&](std::string v) {
								// no need to inform desc_mgmt - it's the end of the block
								_code.emplace_back(asm_code::mov {rax, _const_str_alloc.get_str_addr(v)});
							},
							[&](auto v) {
								// no need to inform desc_mgmt - it's the end of the block
								_code.emplace_back(asm_code::mov {rax, v});
							},
						}, (*e.a).val);
					}
					_code.emplace_back(asm_code::jump {_epilog_label});
				},
			}, i.i.val);

			_desc_mgmt.kill_unused_vars(i.alive_after);
		}

		if (!_epilog_generated)
			gen_basic_block_epilog(b);
	}

	std::ostream &print_code(std::ostream &ost) const {
		return ost << _code;
	}

	const std::vector<asm_code> &get_code() const {
		return _code;
	}

	const std::set<register_type> &get_used_callee_save_regs() const {
		return _desc_mgmt.get_used_callee_save_regs();
	}
};

void add_code(std::vector<asm_code> &dst, std::vector<asm_code> src) {
	for (auto &i : src)
		dst.emplace_back(std::move(i));
}

std::vector<asm_code> gen_function_prolog(std::string func_name, size_t args_num,
		const std::set<register_type> &save_regs) {
	std::vector<asm_code> code;
	code.emplace_back(asm_code::label {std::move(func_name)});
	code.emplace_back(asm_code::push {register_type::RBP});
	code.emplace_back(asm_code::mov {register_type::RBP, register_type::RSP});
	if (args_num > 0) {
		code.emplace_back(
			asm_code::binop {
				core::exp::binary::arithmetic_op_type::SUB,
				register_type::RSP,
				(int)args_num * 8,
			});
	}
	for (auto &reg : save_regs)
		code.emplace_back(asm_code::push {reg});
	return code;
}

std::vector<asm_code> gen_function_epilog(std::string func_name,
		const std::set<register_type> &save_regs) {
	std::vector<asm_code> code;
	code.emplace_back(asm_code::label {gen_epilog_label(std::move(func_name))});
	for (auto it = save_regs.rbegin(); it != save_regs.rend(); ++it)
		code.emplace_back(asm_code::pop {*it});
	code.emplace_back(asm_code::mov {register_type::RSP, register_type::RBP});
	code.emplace_back(asm_code::pop {register_type::RBP});
	code.emplace_back(asm_code::ret {});
	return code;
}

std::vector<asm_code> gen_asm_for_func(const control_flow_graph &cfg, const_str_allocator &const_str_alloc) {
	stack_allocator stack_alloc(cfg.metadata().args);
	std::vector<basic_block_code_generator> c;
	std::set<register_type> save_regs;
	std::vector<asm_code> code;
	for (auto &b : cfg.blocks()) {
		c.emplace_back(b, stack_alloc, const_str_alloc, gen_epilog_label(cfg.metadata().name));
		auto regs = c.back().get_used_callee_save_regs();
		for (auto &r : regs)
			save_regs.emplace(r);
	}
	add_code(code, gen_function_prolog(cfg.metadata().name, stack_alloc.get_local_var_num(), save_regs));
	for (auto &i : c)
		add_code(code, i.get_code());
	add_code(code, gen_function_epilog(cfg.metadata().name, save_regs));
	return code;
}

std::vector<asm_code> gen_program_prolog(const const_str_allocator &const_str_alloc) {
	std::vector<asm_code> code;
	// symbols
	code.emplace_back(asm_code::global_symbol {"_start"});
	code.emplace_back(asm_code::extern_symbol {"_strings_equal"});
	code.emplace_back(asm_code::extern_symbol {"_add_strings"});
	code.emplace_back(asm_code::extern_symbol {"exit"});
	code.emplace_back(asm_code::extern_symbol {"error"});
	code.emplace_back(asm_code::extern_symbol {"printInt"});
	code.emplace_back(asm_code::extern_symbol {"printString"});
	code.emplace_back(asm_code::extern_symbol {"readInt"});
	code.emplace_back(asm_code::extern_symbol {"readString"});
	// .data
	code.emplace_back(asm_code::section {".data"});
	for (auto &i : const_str_alloc.get_str_map())
		code.emplace_back(asm_code::alloc_string {i.second.addr_label, i.first});
	// .text
	code.emplace_back(asm_code::section {".text"});
	// _start
	code.emplace_back(asm_code::label {"_start"});
	code.emplace_back(asm_code::call {"main"});
	code.emplace_back(asm_code::mov {register_type::RDI, register_type::RAX});
	code.emplace_back(asm_code::call {"exit"});
	return code;
}

std::vector<asm_code> gen_asm_for_prog(const std::vector<control_flow_graph> &cfgs) {
	const_str_allocator const_str_alloc;
	std::vector<asm_code> cfg_code;
	for (auto &cfg : cfgs)
		add_code(cfg_code, gen_asm_for_func(cfg, const_str_alloc));
	auto code = gen_program_prolog(const_str_alloc);
	add_code(code, std::move(cfg_code));
	return code;
}

} // namespace latte::backend
