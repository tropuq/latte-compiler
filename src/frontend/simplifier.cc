#include "core/compilation_errors.hh"
#include "core/tree_types.hh"
#include "simplifier.hh"

#include <iostream>
#include <variant>

using namespace latte::core;

namespace latte::frontend {

template<typename T>
void exec_oper_and_update(exp::uptr &e1, exp::uptr &e2, exp::binary::op_type op, decltype(exp::val) &res) {
	auto e1_val = std::get<T>(e1->val);
	auto e2_val = std::get<T>(e2->val);
	visit([&](auto op) {
		auto op_func = get_oper_func<T>(op);
		res = op_func(e1_val, e2_val);
	}, op);
}

template<>
void exec_oper_and_update<int>(exp::uptr &e1, exp::uptr &e2, exp::binary::op_type op, decltype(exp::val) &res) {
	auto e1_val = std::get<int>(e1->val);
	auto e2_val = std::get<int>(e2->val);
	if (e2_val == 0 && std::holds_alternative<exp::binary::arithmetic_op_type>(op)) {
		auto ar_op = std::get<exp::binary::arithmetic_op_type>(op);
		if (ar_op == exp::binary::arithmetic_op_type::DIV || ar_op == exp::binary::arithmetic_op_type::MOD)
			return;
	}

	visit([&](auto op) {
		auto op_func = get_oper_func<int>(op);
		res = op_func(e1_val, e2_val);
	}, op);
}


void simplify_expression_code(exp::uptr &ex) {
	auto is_simple = [](exp::uptr &e) {
		auto &v = e->val;
		return std::holds_alternative<int>(v) ||
			std::holds_alternative<bool>(v) ||
			std::holds_alternative<std::string>(v);
	};
	std::visit(overloaded {
		[&](nested_var &e) {
			std::visit(overloaded {
				[&](func_call &f) {
					for (auto &param : f.params)
						simplify_expression_code(param);
				},
				[&](new_val &n) {
					if (n.arr_size)
						simplify_expression_code(*n.arr_size);
				},
				[](auto &) {}
			}, e.base.val);
			for (auto &field : e.fields) {
				std::visit(overloaded {
					[&](func_call &f) {
						for (auto &param : f.params)
							simplify_expression_code(param);
					},
					[&](nested_var::array_pos &a) {
						simplify_expression_code(a);
					},
					[](auto &) {}
				}, field.val);
			}
		},
		[&](exp::binary &e) {
			// TODO: simplify self == self / null == null etc
			simplify_expression_code(e.e1);
			simplify_expression_code(e.e2);
			// TODO: simplify e2
			if (std::holds_alternative<bool>(e.e1->val) &&
					std::holds_alternative<exp::binary::bool_op_type>(e.op)) {
				bool e1_val = std::get<bool>(e.e1->val);
				bool e2_never_called = false;
				switch (std::get<exp::binary::bool_op_type>(e.op)) {
				case exp::binary::bool_op_type::AND:
					e2_never_called = e1_val == false;
					break;
				case exp::binary::bool_op_type::OR:
					e2_never_called = e1_val == true;
					break;
				}

				if (e2_never_called)
					ex = std::move(e.e1);
				else
					ex = std::move(e.e2);
				return;
			}

			if (is_simple(e.e1) && is_simple(e.e2)) {
				if (std::holds_alternative<int>(e.e1->val))
					exec_oper_and_update<int>(e.e1, e.e2, e.op, ex->val);
				else if (std::holds_alternative<std::string>(e.e1->val))
					exec_oper_and_update<std::string>(e.e1, e.e2, e.op, ex->val);
				else if (std::holds_alternative<bool>(e.e1->val))
					exec_oper_and_update<bool>(e.e1, e.e2, e.op, ex->val);
			}
		},
		[&](exp::unary &e) {
			simplify_expression_code(e.e);
			if (is_simple(e.e)) {
				switch (e.op) {
				case exp::unary::op_type::NOT:
					ex->val = !std::get<bool>(e.e->val);
					break;
				case exp::unary::op_type::NEG:
					ex->val = -std::get<int>(e.e->val);
					break;
				}
			}
		},
		[](auto &) {},
	}, ex->val);
}

void simplify_block_code(block::uptr &bl);

void simplify_statement_code(stmt::uptr &st) {
	std::visit(overloaded {
		[&](stmt::block &bl) {
			simplify_block_code(bl.bl);
		},
		[&](stmt::decl &s) {
			for (auto &item : s.items)
				if (item->init_val)
					simplify_expression_code(*item->init_val);
		},
		[&](stmt::ass &s) {
			simplify_expression_code(s.e);
		},
		[&](stmt::ret &s) {
			if (s.e)
				simplify_expression_code(*s.e);
		},
		[&](stmt::cond &s) {
			simplify_expression_code(s.e);
			if (!std::holds_alternative<bool>(s.e->val)) {
				simplify_statement_code(s.s_true);
				if (s.s_false)
					simplify_statement_code(*s.s_false);
				return;
			} else if (std::get<bool>(s.e->val)) {
				simplify_statement_code(s.s_true);
				st = std::move(s.s_true);
			} else if (s.s_false) {
				simplify_statement_code(*s.s_false);
				st = std::move(*s.s_false);
			} else {
				st->val = stmt::empty {};
			}
		},
		[&](stmt::loop_while &s) {
			simplify_expression_code(s.e);
			if (std::holds_alternative<bool>(s.e->val) && std::get<bool>(s.e->val) == false)
				st->val = stmt::empty {};
			else
				simplify_statement_code(s.s);
		},
		[&](stmt::exp &s) {
			simplify_expression_code(s.e);
		},
		[](auto &) {}
	}, st->val);
}

void simplify_block_code(block::uptr &bl) {
	for (auto &stmt : bl->stmts)
		simplify_statement_code(stmt);
}

bool simplify_block_returns(block::uptr &bl);

// return true if this is the last statement in current block/function
bool simplify_statement_returns(stmt::uptr &st) {
	return std::visit(overloaded {
		[&](stmt::block &bl) {
			return simplify_block_returns(bl.bl);
		},
		[&](stmt::ret &) {
			return true;
		},
		[&](stmt::cond &s) {
			bool ret_true = simplify_statement_returns(s.s_true);
			bool ret_false = s.s_false ? simplify_statement_returns(*s.s_false) : false;
			return ret_true && ret_false;
		},
		[&](stmt::loop_while &s) {
			simplify_statement_returns(s.s);
			return std::holds_alternative<bool>(s.e->val) && std::get<bool>(s.e->val);
		},
		[&](auto &) {
			return false;
		},
	}, st->val);
}

bool simplify_block_returns(block::uptr &bl) {
	for (size_t i = 0; i < bl->stmts.size(); ++i) {
		auto &stmt = bl->stmts[i];
		if (simplify_statement_returns(stmt)) {
			// remove code after return
			while (bl->stmts.size() != i + 1)
				bl->stmts.pop_back();
			return true;
		}
	}
	return false;
}

bool contains_call(const exp::uptr &ex) {
	return std::visit(overloaded {
		[&](nested_var &v) {
			if (std::holds_alternative<func_call>(v.base.val))
				return true;
			for (auto &f : v.fields)
				if (std::holds_alternative<func_call>(f.val))
					return true;
			return false;
		},
		[&](exp::binary &e) {
			return contains_call(e.e1) || contains_call(e.e2);
		},
		[&](exp::unary &e) {
			return contains_call(e.e);
		},
		[&](auto &) {
			return false;
		},
	}, ex->val);
}

void remove_empty_from_block(block::uptr &bl);

void remove_empty_from_statement(stmt::uptr &st) {
	std::visit(overloaded {
		[&](stmt::block &bl) {
			remove_empty_from_block(bl.bl);
			if (bl.bl->stmts.empty())
				st->val = stmt::empty {};
		},
		[&](stmt::cond &s) {
			remove_empty_from_statement(s.s_true);
			if (s.s_false)
				remove_empty_from_statement(*s.s_false);
			bool true_empty = std::holds_alternative<stmt::empty>(s.s_true->val);
			bool false_empty = !s.s_false || std::holds_alternative<stmt::empty>((*s.s_false)->val);
			if (true_empty && false_empty) {
				if (contains_call(s.e))
					st->val = stmt::exp {.e = std::move(s.e)};
				else
					st->val = stmt::empty {};
			} else if (false_empty) {
				s.s_false = std::nullopt;
			} else if (true_empty) {
				s.s_true = std::move(*s.s_false);
				s.s_false = std::nullopt;
				auto neg = std::make_unique<exp>();
				neg->pos = s.e->pos;
				neg->val = exp::unary {
					.e = std::move(s.e),
					.op = exp::unary::op_type::NOT,
				};
				s.e = std::move(neg);
			}
		},
		[&](stmt::loop_while &s) {
			remove_empty_from_statement(s.s);
		},
		[&](stmt::loop_for &s) {
			remove_empty_from_statement(s.s);
		},
		[&](stmt::exp &e) {
			if (!contains_call(e.e))
				st->val = stmt::empty {};
		},
		[](auto &) {}
	}, st->val);
}

void remove_empty_from_block(block::uptr &bl) {
	std::vector<std::unique_ptr<stmt>> new_stmts;
	for (auto &stmt : bl->stmts) {
		remove_empty_from_statement(stmt);
		if (!std::holds_alternative<stmt::empty>(stmt->val))
			new_stmts.emplace_back(std::move(stmt));
	}
	bl->stmts = std::move(new_stmts);
}

void simplify_function_code(func::uptr &fun) {
	return simplify_block_code(fun->bl);
}

void simplify_class_methods_code(class_def::uptr &cls) {
	for (auto &def : cls->defs)
		if (std::holds_alternative<func::uptr>(def))
			simplify_function_code(std::get<func::uptr>(def));
}

void simplify_function_returns(func::uptr &fun) {
	if (!simplify_block_returns(fun->bl) && fun->tp.val != type::single_type::type_enum::VOID)
		throw compilation_error(fun->pos, concat("non void function '", fun->id, "' doesn't return"));
}

void simplify_class_methods_returns(class_def::uptr &cls) {
	for (auto &def : cls->defs)
		if (std::holds_alternative<func::uptr>(def))
			simplify_function_returns(std::get<func::uptr>(def));
}

void remove_empty_from_function(func::uptr &fun) {
	return remove_empty_from_block(fun->bl);
}

void remove_empty_from_class_methods(class_def::uptr &cls) {
	for (auto &def : cls->defs)
		if (std::holds_alternative<func::uptr>(def))
			remove_empty_from_function(std::get<func::uptr>(def));
}

void simplify_program(program::uptr &prog) {
	// simplify code
	for (auto &def : prog->defs) {
		std::visit(overloaded { // TODO: less copy pasting
			[](func::uptr &fun) {
				simplify_function_code(fun);
			},
			[](class_def::uptr &cls) {
				simplify_class_methods_code(cls);
			}
		}, def);
	}

	// simplify returns (and perform some checks)
	for (auto &def : prog->defs) {
		std::visit(overloaded {
			[](func::uptr &fun) {
				simplify_function_returns(fun);
			},
			[](class_def::uptr &cls) {
				simplify_class_methods_returns(cls);
			}
		}, def);
	}

	// remove most of the empty statements
	for (auto &def : prog->defs) {
		std::visit(overloaded {
			[](func::uptr &fun) {
				remove_empty_from_function(fun);
			},
			[](class_def::uptr &cls) {
				remove_empty_from_class_methods(cls);
			}
		}, def);
	}

	// TODO: remove unused functions
}

} // namespace latte::frontend
