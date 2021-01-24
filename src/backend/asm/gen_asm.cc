#include "asm_code.hh"
#include "backend/ic/control_flow_graph.hh"
#include "const_string_allocator.hh"
#include "core/tree_types.hh"
#include "description_manager.hh"
#include "remove_dead_code.hh"
#include "stack_allocator.hh"

#include <cassert>
#include <map>
#include <optional>
#include <string>
#include <variant>
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

	void prepare_locs_for_bin_op(const instr::rvalue &rv1, const instr::rvalue &rv2, register_type &dst,
			asm_code::snd_param_type &src, const std::set<core::ident> &alive_after) {
		std::visit(overloaded {
			[&](const instr::variable_id &v) {
				dst = _desc_mgmt.load_var_to_reg(v.id);
			},
			[&](const instr::address_offset &v) {
				dst = load_address_offset_value_to_reg(v);
			},
			[&](const instr::null&) {
				dst = _desc_mgmt.get_free_reg();
				_code.emplace_back(asm_code::mov {dst, 0});
			},
			[&](const int &v) {
				dst = _desc_mgmt.get_free_reg();
				_code.emplace_back(asm_code::mov {dst, v});
			},
			[&](const bool &v) {
				dst = _desc_mgmt.get_free_reg();
				_code.emplace_back(asm_code::mov {dst, v});
			},
			[&](auto &) {
				assert(false);
			},
		}, rv1.val);

		std::visit(overloaded {
			[&](const instr::variable_id &v) {
				if (alive_after.find(v.id) != alive_after.end())
					_desc_mgmt.try_load_var_to_reg(v.id);
				src = _desc_mgmt.get_var_loc(v.id);
			},
			[&](const instr::address_offset &v) {
				src = convert_address_offset(v);
			},
			[&](const instr::null&) {
				src = 0;
			},
			[&](const int &v) {
				src = v;
			},
			[&](const bool &v) {
				src = v;
			},
			[&](auto &) {
				assert(false);
			},
		}, rv2.val);
	}

	void perform_div_op(const instr::rvalue &rv1, const instr::rvalue &rv2, core::exp::binary::arithmetic_op_type op,
			const instr::lvalue &lv, const std::set<core::ident> &alive_after) {
		auto rdx = register_type::RDX;
		auto rax = register_type::RAX;
		_desc_mgmt.forbid_reg_spill_dest_for_cur_instr(rdx);
		_desc_mgmt.forbid_reg_spill_dest_for_cur_instr(rax);

		_desc_mgmt.get_free_reg(rdx);
		std::visit(overloaded {
			[&](const instr::variable_id &v) {
				_desc_mgmt.load_var_to_reg(v.id, rax);
			},
			[&](const instr::address_offset &v) {
				load_address_offset_value_to_reg(v, rax);
			},
			[&](const int &v) {
				_desc_mgmt.get_free_reg(rax);
				_code.emplace_back(asm_code::mov {rax, v});
			},
			[&](auto &) {
				assert(false);
			},
		}, rv1.val);

		asm_code::snd_param_type src;
		std::optional<register_type> free_reg;
		std::visit(overloaded {
			[&](const instr::variable_id &v) {
				if (alive_after.find(v.id) != alive_after.end())
					_desc_mgmt.try_load_var_to_reg(v.id);
				src = _desc_mgmt.get_var_loc(v.id);
			},
			[&](const instr::address_offset &v) {
				src = convert_address_offset(v);
			},
			[&](const int &v) {
				free_reg = _desc_mgmt.get_free_reg();
				_code.emplace_back(asm_code::mov {*free_reg, v});
				src = *free_reg;
			},
			[&](auto &) {
				assert(false);
			},
		}, rv2.val);

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

		associate_modified_reg_with_lvalue(dst, lv);
		_desc_mgmt.free_reg(free);
		if (free_reg)
			_desc_mgmt.free_reg(*free_reg);
	}

	void gen_basic_block_epilog(const basic_block &b) {
		_epilog_generated = true;
		return _desc_mgmt.move_vars_to_memory(b.get_end_alive());
	}

	register_offset convert_address_offset(const instr::address_offset &r) {
		auto reg1 = _desc_mgmt.load_var_to_reg(r.var.id);
		if (r.index) {
			auto reg2 = _desc_mgmt.load_var_to_reg(r.index->id);
			return register_offset {reg1, (int64_t)r.offset * 8, register_offset::index_data {reg2, 8}};
		}
		return register_offset {reg1, (int64_t)r.offset * 8, std::nullopt};
	}

	register_type load_address_offset_value_to_reg(const instr::address_offset &r) {
		auto reg = _desc_mgmt.get_free_reg(); // TODO: possibly choose reg from convert_address_offset
		_code.emplace_back(asm_code::mov {reg, convert_address_offset(r)});
		return reg;
	}

	void load_address_offset_value_to_reg(const instr::address_offset &r, register_type reg) {
		_desc_mgmt.get_free_reg(reg);
		_code.emplace_back(asm_code::mov {reg, convert_address_offset(r)});
	}

	void associate_reg_with_lvalue(register_type reg, const instr::lvalue &lv) {
		std::visit(overloaded {
			[&](const instr::address_offset &a) {
				_code.emplace_back(asm_code::mov {convert_address_offset(a), reg});
			},
			[&](const instr::variable_id &v) {
				_desc_mgmt.associate_reg_with_var(reg, v.id);
			}
		}, lv.val);
	}

	void associate_modified_reg_with_lvalue(register_type reg, const instr::lvalue &lv) {
		std::visit(overloaded {
			[&](const instr::address_offset &a) {
				_code.emplace_back(asm_code::mov {convert_address_offset(a), reg});
				_desc_mgmt.free_reg(reg);
			},
			[&](const instr::variable_id &v) {
				_desc_mgmt.associate_reg_with_modified_var(reg, v.id);
			}
		}, lv.val);
	}

public:
	basic_block_code_generator(const basic_block &b, stack_allocator &stack_alloc,
			const_str_allocator &const_str_alloc, std::string epilog_label)
			: _const_str_alloc(const_str_alloc), _desc_mgmt(_code, stack_alloc, _const_str_alloc, b.get_beg_alive()),
			_epilog_label(std::move(epilog_label)) {
		for (auto &i : b.get_instr_info()) {
			std::visit(overloaded {
				[&](const instr::prepare_call_arg &e) {
					std::visit(overloaded {
						[&](const instr::variable_id &v) {
							if (i.alive_after.find(v.id) != i.alive_after.end())
								_desc_mgmt.try_load_var_to_reg(v.id);
							_code.emplace_back(asm_code::push {_desc_mgmt.get_var_loc(v.id)});
						},
						[&](const instr::fixed_address &v) {
							_code.emplace_back(asm_code::push {fixed_address {v.addr}});
						},
						[&](const instr::address_offset &v) {
							_code.emplace_back(asm_code::push {convert_address_offset(v)});
						},
						[&](const std::string &v) {
							_code.emplace_back(asm_code::push {fixed_address {_const_str_alloc.get_str_addr(v)}});
						},
						[&](const instr::null&) {
							_code.emplace_back(asm_code::push {0});
						},
						[&](auto &v) {
							_code.emplace_back(asm_code::push {v});
						},
					}, e.rv.val);
				},
				[&](const instr::call_ass &e) {
					auto rax = register_type::RAX;
					_desc_mgmt.get_free_reg(rax);
					std::visit(overloaded {
						[&](const instr::variable_id &v) {
							_code.emplace_back(asm_code::call {v.id});
							associate_modified_reg_with_lvalue(rax, e.lv);
						},
						[&](const instr::address_offset &a) {
							_code.emplace_back(asm_code::call {convert_address_offset(a)});
							associate_modified_reg_with_lvalue(rax, e.lv);
						},
					}, e.func_hdl.val);

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
				[&](const instr::bin_ass &e) {
					// Because div/mod operations are different.
					if (e.op == core::exp::binary::arithmetic_op_type::MOD ||
							e.op == core::exp::binary::arithmetic_op_type::DIV) {
						perform_div_op(e.rv1, e.rv2, e.op, e.lv, i.alive_after);
						return;
					}

					register_type dst;
					asm_code::snd_param_type src;
					prepare_locs_for_bin_op(e.rv1, e.rv2, dst, src, i.alive_after);

					_desc_mgmt.detach_reg(dst, i.alive_after);
					_code.emplace_back(asm_code::binop {e.op, dst, src});
					associate_modified_reg_with_lvalue(dst, e.lv);
				},
				[&](const instr::unary_ass &e) {
					register_type reg;
					std::visit(overloaded {
						[&](const instr::variable_id &v) {
							reg = _desc_mgmt.load_var_to_reg(v.id);
							_desc_mgmt.detach_reg(reg, i.alive_after);
						},
						[&](const instr::address_offset &v) {
							reg = load_address_offset_value_to_reg(v);
						},
						[&](const int &v) {
							reg = _desc_mgmt.get_free_reg();
							_code.emplace_back(asm_code::mov {reg, v});
						},
						[&](const bool &v) {
							reg = _desc_mgmt.get_free_reg();
							_code.emplace_back(asm_code::mov {reg, v});
						},
						[&](auto &) {
							assert(false);
						},
					}, e.rv.val);
					switch (e.op) {
					case core::exp::unary::op_type::NEG:
						_code.emplace_back(asm_code::neg {reg});
						break;
					case core::exp::unary::op_type::NOT:
						_code.emplace_back(asm_code::bool_not {reg});
						break;
					}
					associate_modified_reg_with_lvalue(reg, e.lv);
				},
				[&](const instr::set &e) {
					std::visit(overloaded {
						[&](const instr::variable_id &v) {
							// load to register or copy existing register
							auto reg = _desc_mgmt.load_var_to_reg(v.id);
							associate_reg_with_lvalue(reg, e.lv);
						},
						[&](const instr::fixed_address &v) {
							auto reg = _desc_mgmt.get_free_reg();
							_code.emplace_back(asm_code::mov {reg, fixed_address {v.addr}});
							associate_modified_reg_with_lvalue(reg, e.lv);
						},
						[&](const instr::address_offset &v) {
							auto reg = load_address_offset_value_to_reg(v);
							associate_modified_reg_with_lvalue(reg, e.lv);
						},
						[&](const std::string &v) {
							auto reg = _desc_mgmt.load_string_to_reg(v);
							associate_modified_reg_with_lvalue(reg, e.lv);
						},
						[&](const instr::null&) {
							auto reg = _desc_mgmt.get_free_reg();
							_code.emplace_back(asm_code::mov {reg, 0});
							associate_modified_reg_with_lvalue(reg, e.lv);
						},
						[&](auto &v) {
							auto reg = _desc_mgmt.get_free_reg();
							_code.emplace_back(asm_code::mov {reg, v});
							associate_modified_reg_with_lvalue(reg, e.lv);
						},
					}, e.rv.val);
				},
				[&](const instr::jump &e) {
					gen_basic_block_epilog(b);
					_code.emplace_back(asm_code::jump {e.label});
				},
				[&](const instr::cond_jump &e) {
					register_type l;
					asm_code::snd_param_type r;
					prepare_locs_for_bin_op(e.rv1, e.rv2, l, r, i.alive_after);

					_code.emplace_back(asm_code::cmp {l, r});

					gen_basic_block_epilog(b);

					_code.emplace_back(asm_code::cond_jump {e.op, e.label_true});
				},
				[&](const instr::label &e) {
					_code.emplace_back(asm_code::label {e.name});
				},
				[&](const instr::ret &e) {
					gen_basic_block_epilog(b);
					if (e.val) {
						auto rax = register_type::RAX;
						std::visit(overloaded {
							[&](const instr::variable_id &v) {
								_desc_mgmt.load_var_to_reg(v.id, rax);
							},
							[&](const instr::fixed_address &v) {
								_code.emplace_back(asm_code::mov {rax, fixed_address {v.addr}});
							},
							[&](const instr::address_offset &v) {
								_code.emplace_back(asm_code::mov {rax, convert_address_offset(v)});
							},
							[&](const std::string &v) {
								_code.emplace_back(asm_code::mov {rax, _const_str_alloc.get_str_addr(v)});
							},
							[&](const instr::null&) {
								_code.emplace_back(asm_code::mov {rax, 0});
							},
							[&](auto &v) {
								_code.emplace_back(asm_code::mov {rax, v});
							},
						}, e.val->val);
					}
					_code.emplace_back(asm_code::jump {_epilog_label});
				},
			}, i.i.val);

			_desc_mgmt.end_process_cur_instr(i.alive_after);

			_desc_mgmt.check();
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
	std::string epilog_label = gen_epilog_label(cfg.metadata().name);
	for (auto &b : cfg.blocks()) {
		c.emplace_back(b, stack_alloc, const_str_alloc, epilog_label);
		auto regs = c.back().get_used_callee_save_regs();
		for (auto &r : regs)
			save_regs.emplace(r);
	}
	add_code(code, gen_function_prolog(cfg.metadata().name, stack_alloc.get_local_var_num(), save_regs));
	for (auto &i : c) {
		auto basic_block_code = i.get_code();
		remove_dead_code(basic_block_code, epilog_label);
		add_code(code, std::move(basic_block_code));
	}
	add_code(code, gen_function_epilog(cfg.metadata().name, save_regs));
	return code;
}

std::vector<asm_code> gen_program_prolog(const const_str_allocator &const_str_alloc,
		const std::vector<vtable_description> &vtables) {
	std::vector<asm_code> code;
	// symbols
	code.emplace_back(asm_code::global_symbol {"_start"});
	code.emplace_back(asm_code::extern_symbol {"_strings_equal"});
	code.emplace_back(asm_code::extern_symbol {"_add_strings"});
	code.emplace_back(asm_code::extern_symbol {"_alloc"});
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
	for (auto &v : vtables)
		code.emplace_back(asm_code::define_vtable {v.vtable_addr_name, v.methods_addr_name});
	// .text
	code.emplace_back(asm_code::section {".text"});
	// _start
	code.emplace_back(asm_code::label {"_start"});
	code.emplace_back(asm_code::call {"main"});
	code.emplace_back(asm_code::mov {register_type::RDI, register_type::RAX});
	code.emplace_back(asm_code::call {"exit"});
	return code;
}

std::vector<asm_code> gen_asm_for_prog(const program_graph &p) {
	const_str_allocator const_str_alloc;
	std::vector<asm_code> cfg_code;
	for (auto &cfg : p.control_flow_graphs())
		add_code(cfg_code, gen_asm_for_func(cfg, const_str_alloc));
	auto code = gen_program_prolog(const_str_alloc, p.vtables());
	add_code(code, std::move(cfg_code));
	return code;
}

} // namespace latte::backend
