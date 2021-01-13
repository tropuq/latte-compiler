#include "core/compilation_errors.hh"
#include "core/tree_types.hh"
#include "utils/utils.hh"
#include "verifier.hh"

#include <map>
#include <set>
#include <stdexcept>
#include <iostream>
#include <variant>

using namespace latte::core;

namespace latte::frontend {

class func_map {
	std::vector<func::uptr> _predef_func;
	std::map<ident, func*> _func_map;
public:
	void add_func(func::uptr &f) {
		bool success = _func_map.emplace(f->id, f.get()).second;
		if (!success)
			throw compilation_error(concat("function ", f->id, "() already defined"));
	}

	func* find_func(const ident &id) const {
		auto it = _func_map.find(id);
		return it == _func_map.end() ? nullptr : it->second;
	}

	void add_predef_func(ident id, type::type_enum ret_type, std::vector<type::type_enum> args_types = {}) {
		auto f = std::make_unique<func>();
		f->id = move(id);
		f->tp.val = ret_type;
		for (auto &arg_type : args_types) {
			arg a;
			a.tp.val = arg_type;
			f->args.emplace_back(a);
		}
		f->tp.val = ret_type;
		add_func(f);
		_predef_func.emplace_back(move(f));
	}
};

func_map prepare_func_map(const program::uptr &prog) {
	func_map func_map;
	func_map.add_predef_func("printInt", type::type_enum::VOID, {type::type_enum::INT});
	func_map.add_predef_func("printString", type::type_enum::VOID, {type::type_enum::STR});
	func_map.add_predef_func("error", type::type_enum::VOID);
	func_map.add_predef_func("readInt", type::type_enum::INT);
	func_map.add_predef_func("readString", type::type_enum::STR);

	for (auto &f : prog->funcs) {
		try {
			func_map.add_func(f);
		} catch (compilation_error &err) {
			err.error_pos = f->pos;
			throw;
		}
	}

	func *main_f = func_map.find_func("main");
	if (main_f == nullptr)
		throw compilation_error("function main() undefined");

	if (main_f->tp.val != type::type_enum::INT)
		throw compilation_error(main_f->tp.pos, "main() should return int");

	if (!main_f->args.empty())
		throw compilation_error(main_f->args[0].pos, "main() shouldn't accept any arguments");

	return func_map;
}

class var_map {
	std::vector<std::set<ident>> _block_vars;
	std::map<ident, std::vector<type::type_enum>> _types_stack;
public:
	void push_block() {
		_block_vars.emplace_back();
	}

	void push_var(const ident &id, type::type_enum tp) {
		if (tp == type::type_enum::VOID)
			throw compilation_error(concat("variable '", id, "' cannot be of type void"));
		bool success = _block_vars.back().emplace(id).second;
		if (!success)
			throw compilation_error(concat("redefinition of '", id, "'"));
		_types_stack[id].emplace_back(tp);

	}

	void pop_block() {
		for (auto &id : _block_vars.back()) {
			auto it = _types_stack.find(id);
			it->second.pop_back();
			if (it->second.empty())
				_types_stack.erase(it);
		}
		_block_vars.pop_back();
	}

	type::type_enum get_var_type(const ident &id) const {
		auto it = _types_stack.find(id);
		if (it == _types_stack.end())
			throw compilation_error(concat("use of undeclared identifier '", id, "'"));
		return it->second.back();
	}
};

type::type_enum verify_exp_code(exp::uptr &ex, const func_map &func_map, const var_map &var_map) {
	auto ret = visit(overloaded {
		[&](exp::var &e) {
			try {
				return var_map.get_var_type(e.id);
			} catch (compilation_error &err) {
				err.error_pos = ex->pos;
				throw;
			}
		},
		[&](exp::call &e) {
			func *fun_ptr = func_map.find_func(e.id);
			if (fun_ptr == nullptr)
				throw compilation_error(ex->pos, concat("use of undeclared identifier '", e.id, "'"));
			if (fun_ptr->args.size() != e.params.size())
				throw compilation_error(ex->pos, concat("invalid number of arguments - expected ",
					fun_ptr->args.size(), " got ", e.params.size()));
			for (size_t i = 0; i < e.params.size(); ++i) {
				auto &param = e.params[i];
				auto &arg = fun_ptr->args[i];
				auto param_tp = verify_exp_code(param, func_map, var_map);
				if (param_tp != arg.tp.val)
					throw compilation_error(param->pos, concat("invalid type of argument '", param_tp,
						"' instead of '", arg.tp.val, "'"));
			}
			return fun_ptr->tp.val;
		},
		[&](exp::binary &e) {
			auto exp1_tp = verify_exp_code(e.e1, func_map, var_map);
			auto exp2_tp = verify_exp_code(e.e2, func_map, var_map);
			if (exp1_tp != exp2_tp)
				throw compilation_error(ex->pos, concat("invalid type of arguments in binary expression '",
					exp1_tp, "' and '", exp2_tp, "'"));
			bool is_eq_neq = is_same_oper(e.op, exp::binary::rel_op_type::EQU) ||
				is_same_oper(e.op, exp::binary::rel_op_type::NE);
			bool is_add = is_same_oper(e.op, exp::binary::arithmetic_op_type::ADD);
			switch (exp1_tp) {
			case type::type_enum::INT:
				if (is_arithmetic_oper(e.op))
					return type::type_enum::INT;
				if (is_rel_oper(e.op))
					return type::type_enum::BOOL;
				break;
			case type::type_enum::BOOL:
				if (is_bool_oper(e.op) || is_eq_neq)
					return type::type_enum::BOOL;
				break;
			case type::type_enum::STR:
				if (is_add)
					return type::type_enum::STR;
				if (is_eq_neq)
					return type::type_enum::BOOL;
				break;
			case type::type_enum::VOID:
				throw compilation_error(ex->pos, "void cannot occur in binary expression");
			}
			throw compilation_error(ex->pos, concat("invalid operator '", e.op,
				"' in binary expression with expressions of type '", exp1_tp, "'"));
		},
		[&](exp::unary &e) {
			auto exp_tp = verify_exp_code(e.e, func_map, var_map);
			type::type_enum expected_type;
			switch (e.op) {
			case exp::unary::op_type::NOT:
				expected_type = type::type_enum::BOOL;
				break;
			case exp::unary::op_type::NEG:
				expected_type = type::type_enum::INT;
				break;
			}
			if (exp_tp != expected_type)
				throw compilation_error(e.e->pos,
					concat("invalid type of expression for unary operation - expected '",
						expected_type, "' instead of '", exp_tp, "'"));
			return exp_tp;
		},
		[&](int &) {
			return type::type_enum::INT;
		},
		[&](bool &) {
			return type::type_enum::BOOL;
		},
		[&](std::string &) {
			return type::type_enum::STR;
		},
	}, ex->val);
	return ret;
}

void ensure_new_block(stmt::uptr &st) {
	if (std::holds_alternative<stmt::block>(st->val))
		return;
	auto new_bl = std::make_unique<block>();
	new_bl->pos = st->pos;

	auto inner_st = std::make_unique<stmt>();
	inner_st->pos = st->pos;
	inner_st->val = move(st->val);
	new_bl->stmts.emplace_back(move(inner_st));

	st->val = stmt::block {
		.bl = move(new_bl),
	};
}

void verify_block_code(block::uptr &bl, const func_map &func_map, var_map &var_map,
	type::type_enum func_tp, bool push_new_block = true);

void verify_statement_code(stmt::uptr &st, const func_map &func_map, var_map &var_map, type::type_enum func_tp) {
	auto try_get_var_type = [&var_map](const ident &id, tree_node *node) {
		try {
			return var_map.get_var_type(id);
		} catch (compilation_error &err) {
			err.error_pos = node->pos;
			throw;
		}
	};
	visit(overloaded {
		[&](stmt::block &bl) {
			verify_block_code(bl.bl, func_map, var_map, func_tp);
		},
		[&](stmt::decl &s) {
			for (auto &item : s.items) {
				try {
					var_map.push_var(item->id, s.tp.val);
				} catch (compilation_error &err) {
					err.error_pos = item->pos;
					throw;
				}
				if (item->init_val) {
					auto exp_tp = verify_exp_code(*item->init_val, func_map, var_map);
					if (s.tp.val != exp_tp)
						throw compilation_error((*item->init_val)->pos,
							concat("cannot initialize a variable of type '", s.tp.val,
								"' with expression of type '", exp_tp, "'"));
				}
			}
		},
		[&](stmt::ass &s) {
			type::type_enum var_tp = try_get_var_type(s.id, st.get());
			auto exp_tp = verify_exp_code(s.e, func_map, var_map);
			if (var_tp != exp_tp)
				throw compilation_error(s.e->pos, concat("cannot assign expression of type '", exp_tp,
					"' to variable of type '", var_tp, "'"));
		},
		[&](stmt::unary &s) {
			type::type_enum tp = try_get_var_type(s.id, st.get());
			if (tp != type::type_enum::INT)
				throw compilation_error(st->pos,
					concat("invalid type of '", s.id, "' for this operation - should be 'int'"));
		},
		[&](stmt::ret &s) {
			if (s.e) {
				auto exp_tp = verify_exp_code(*s.e, func_map, var_map);
				if (func_tp != exp_tp)
					throw compilation_error(st->pos,
						concat("invalid type of return expression '", exp_tp,
							"' - should be '", func_tp, "'"));
			} else {
				if (func_tp != type::type_enum::VOID)
					throw compilation_error(st->pos,
						concat("invalid type of return expression 'void' - should be '",
							func_tp, "'"));
			}
		},
		[&](stmt::cond &s) {
			auto exp_tp = verify_exp_code(s.e, func_map, var_map);
			if (exp_tp != type::type_enum::BOOL)
				throw compilation_error(s.e->pos, concat("invalid type of condition expression '",
					exp_tp, "' - should be 'boolean'"));
			ensure_new_block(s.s_true);
			verify_statement_code(s.s_true, func_map, var_map, func_tp);
			if (s.s_false) {
				ensure_new_block(*s.s_false);
				verify_statement_code(*s.s_false, func_map, var_map, func_tp);
			}
		},
		[&](stmt::loop_while &s) {
			auto exp_tp = verify_exp_code(s.e, func_map, var_map);
			if (exp_tp != type::type_enum::BOOL)
				throw compilation_error(s.e->pos, concat("invalid type of condition expression '",
					exp_tp, "' - should be 'boolean'"));
			ensure_new_block(s.s);
			verify_statement_code(s.s, func_map, var_map, func_tp);
		},
		[&](stmt::exp &s) {
			verify_exp_code(s.e, func_map, var_map);
		},
		[&](stmt::empty &) {},
	}, st->val);
}

void verify_block_code(block::uptr &bl, const func_map &func_map, var_map &var_map,
	type::type_enum func_tp, bool push_new_block)
{
	if (push_new_block)
		var_map.push_block();
	for (auto &stmt : bl->stmts)
		verify_statement_code(stmt, func_map, var_map, func_tp);
	var_map.pop_block();
}

void verify_program(program::uptr &prog) {
	auto func_map = prepare_func_map(prog);
	for (auto &fun : prog->funcs) {
		var_map var_map;
		var_map.push_block();
		for (auto &arg : fun->args) {
			try {
				var_map.push_var(arg.id, arg.tp.val);
			} catch (compilation_error &err) {
				err.error_pos = arg.pos;
				throw;
			}
		}
		verify_block_code(fun->bl, func_map, var_map, fun->tp.val, false);
	}
}

} // namespace latte::frontend
