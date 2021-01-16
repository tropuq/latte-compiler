#include "core/compilation_errors.hh"
#include "core/tree_types.hh"
#include "utils/utils.hh"
#include "verifier.hh"

#include <map>
#include <set>
#include <stdexcept>
#include <iostream>
#include <variant>
#include <cassert>

using namespace latte::core;

namespace latte::frontend {

class func_map {
	std::map<ident, func*> _func_map;
public:
	void add_func(func::uptr &f) {
		bool success = _func_map.emplace(f->id, f.get()).second;
		if (!success)
			throw compilation_error(concat("function ", f->id, "() already defined"));
	}

	void redefine_func(func::uptr &f) {
		if (_func_map.find(f->id) == _func_map.end()) // TODO: check if signatture is the same
			return add_func(f);
		_func_map[f->id] = f.get();
	}

	func* find_func(const ident &id) const {
		auto it = _func_map.find(id);
		return it == _func_map.end() ? nullptr : it->second;
	}
};

class global_func_map : public func_map {
	std::vector<func::uptr> _predef_func;
public:
	void add_predef_func(ident id, type::val_type ret_type, std::vector<type::val_type> args_types = {}) {
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

class var_map {
	std::vector<std::set<ident>> _block_vars;
	std::map<ident, std::vector<type::val_type>> _types_stack;
public:
	void push_block() {
		_block_vars.emplace_back();
	}

	void push_var(const ident &id, type::val_type tp) {
		if (get_single_type(tp) == type::single_type {type::single_type::type_enum::VOID})
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

	type::val_type get_var_type(const ident &id) const {
		auto it = _types_stack.find(id);
		if (it == _types_stack.end())
			throw compilation_error(concat("use of undeclared identifier '", id, "'"));
		return it->second.back();
	}
};

class class_map {
	struct class_data {
		func_map methods;
		std::set<core::ident> local_methods;
		std::map<core::ident, class_def::field*> fields;
		std::set<core::ident> upper_classes;
	};
	std::map<core::ident, class_data> _cls_data; // class name -> func_map

public:
	void add_extended_class(const ident &class_id, const ident &extends_id) {
		if (_cls_data.find(class_id) != _cls_data.end())
			throw compilation_error(concat("class '", class_id, "' redefined"));
		if (_cls_data.find(extends_id) == _cls_data.end())
			throw compilation_error(concat("extended class '", extends_id, "' not defined"));
		_cls_data[class_id] = _cls_data[extends_id];
		_cls_data[class_id].upper_classes.emplace(extends_id);
		_cls_data[class_id].local_methods.clear();
	}

	void add_class(const ident &class_id) {
		if (_cls_data.find(class_id) != _cls_data.end())
			throw compilation_error(concat("class '", class_id, "' redefined"));
		_cls_data[class_id];
	}

	void add_method(const ident &class_id, func::uptr &f) {
		auto it = _cls_data.find(class_id);
		assert(it != _cls_data.end());
		if (it->second.local_methods.find(f->id) != it->second.local_methods.end())
			throw compilation_error(concat("class method '", f->id, "' redefined"));
		it->second.local_methods.emplace(f->id);
		it->second.methods.redefine_func(f);
	}

	func* find_method(const ident &class_id, const ident &method_id) const {
		auto it = _cls_data.find(class_id);
		assert(it != _cls_data.end());
		return it->second.methods.find_func(method_id);
	}

	void add_field(const ident &class_id, class_def::field* f) {
		auto class_it = _cls_data.find(class_id);
		assert(class_it != _cls_data.end());
		if (class_it->second.fields.find(f->id) != class_it->second.fields.end())
			throw compilation_error(concat("class field '", f->id, "' redefined"));
		class_it->second.fields.emplace(f->id, f);
	}

	class_def::field* find_field(const ident &class_id, const ident &field_id) const {
		auto it = _cls_data.find(class_id);
		assert(it != _cls_data.end());
		auto field_it = it->second.fields.find(field_id);
		return field_it == it->second.fields.end() ? nullptr : field_it->second;
	}

	const std::map<core::ident, class_def::field*> &get_class_fields_map(const ident &class_id) const {
		auto it = _cls_data.find(class_id);
		assert(it != _cls_data.end());
		return it->second.fields;
	}

	bool does_class_extend(const ident &lower, const ident &upper) const {
		auto it = _cls_data.find(lower);
		assert(it != _cls_data.end());
		return it->second.upper_classes.find(upper) != it->second.upper_classes.end();
	}

	bool does_class_exists(const ident &class_id) {
		return _cls_data.find(class_id) != _cls_data.end();
	}
};

struct verifier_data {
	global_func_map glob_funcs;
	class_map cls_map;
	var_map vars;
	func *cur_fun = nullptr;
	class_def *cur_cls = nullptr;
};

void prepare_verifier_data(const program::uptr &prog, verifier_data &vf_data) {
	using st = type::single_type;
	vf_data.glob_funcs.add_predef_func("printInt", st {st::type_enum::VOID}, {st {st::type_enum::INT}});
	vf_data.glob_funcs.add_predef_func("printString", st {st::type_enum::VOID}, {st {st::type_enum::STR}});
	vf_data.glob_funcs.add_predef_func("error", st {st::type_enum::VOID});
	vf_data.glob_funcs.add_predef_func("readInt", st {st::type_enum::INT});
	vf_data.glob_funcs.add_predef_func("readString", st {st::type_enum::STR});

	for (auto &d : prog->defs) {
		visit(overloaded {
			[&](func::uptr &f) {
				try {
					vf_data.glob_funcs.add_func(f);
				} catch (compilation_error &err) {
					err.error_pos = f->pos;
					throw;
				}
			},
			[&](class_def::uptr &cls) {
				try {
					if (cls->extends_id)
						vf_data.cls_map.add_extended_class(cls->class_id, *cls->extends_id);
					else
						vf_data.cls_map.add_class(cls->class_id);
				} catch (compilation_error &err) {
					err.error_pos = cls->pos;
					throw;
				}
				for (auto &cd : cls->defs) {
					code_pos pos;
					try {
						visit(overloaded {
							[&](func::uptr &f) {
								pos = f->pos;
								vf_data.cls_map.add_method(cls->class_id, f);
							},
							[&](class_def::field &f) {
								pos = f.pos;
								vf_data.cls_map.add_field(cls->class_id, &f);
							}
						}, cd);
					} catch (compilation_error &err) {
						err.error_pos = pos;
						throw;
					}
				}
			}
		}, d);
	}

	func *main_f = vf_data.glob_funcs.find_func("main");
	if (main_f == nullptr)
		throw compilation_error("function main() undefined");

	if (main_f->tp.val != st {st::type_enum::INT})
		throw compilation_error(main_f->tp.pos, "main() should return int");

	if (!main_f->args.empty())
		throw compilation_error(main_f->args[0].pos, "main() shouldn't accept any arguments");
}

std::optional<type::single_type::class_type> try_get_class_tp(type::val_type t) {
	if (!std::holds_alternative<type::single_type>(t))
		return std::nullopt;
	auto &single_tp = std::get<type::single_type>(t);
	if (!std::holds_alternative<type::single_type::class_type>(single_tp.val))
		return std::nullopt;
	return std::get<type::single_type::class_type>(single_tp.val);
};

bool can_convert_type(const type::val_type &from, const type::val_type &to, verifier_data &vf_data) {
	if (from == to)
		return true;
	auto to_class = try_get_class_tp(to);
	auto is_from_null_type = from == type::single_type {type::single_type::null_type {}};
	if (!to_class) {
		if (is_from_null_type && std::holds_alternative<type::array_type>(to))
			return true;
		return false;
	}
	auto from_class = try_get_class_tp(from);
	if (!from_class) {
		if (is_from_null_type)
			return true;
		return false;
	}
	return vf_data.cls_map.does_class_extend(from_class->id, to_class->id);
}

type::val_type verify_exp_code(exp::uptr &ex, verifier_data &vf_data);

void verify_call_args(func *func, func_call &call, verifier_data &vf_data, code_pos call_pos) {
	assert(func != nullptr);
	if (func->args.size() != call.params.size())
		throw compilation_error(call_pos, concat("invalid number of arguments - expected ",
			func->args.size(), " got ", call.params.size()));
	for (size_t i = 0; i < call.params.size(); ++i) {
		auto &param = call.params[i];
		auto &arg = func->args[i];
		auto param_tp = verify_exp_code(param, vf_data);
		if (!can_convert_type(param_tp, arg.tp.val, vf_data))
			throw compilation_error(param->pos, concat("invalid type of argument '", param_tp,
				"' instead of '", arg.tp.val, "'"));
	}
}

void verify_single_type(const type::single_type &t, verifier_data &vf_data) {
	std::visit(overloaded {
		[](const type::single_type::null_type &) {},
		[](const type::single_type::type_enum &) {},
		[&](const type::single_type::class_type &c) {
			if (!vf_data.cls_map.does_class_exists(c.id))
				throw compilation_error(concat("class '", c.id, "' doesn't exist"));
		}
	}, t.val);
}

void verify_type(const type::val_type &tp, verifier_data &vf_data) {
	std::visit(overloaded {
		[&](const type::single_type &s) {
			verify_single_type(s, vf_data);
		},
		[&](const type::array_type &a) {
			verify_single_type(a.val, vf_data);
		}
	}, tp);
}

struct get_nested_var_type_return {
	type::val_type tp;
	bool assignable;
};

get_nested_var_type_return get_nested_var_type(nested_var &v, verifier_data &vf_data) {
	type::val_type type;
	bool assignable;
	// get base types
	std::visit(overloaded {
		[&](nested_var::self &) {
			assignable = false;
			if (vf_data.cur_cls == nullptr)
				throw compilation_error(v.base.pos, "self outside class");
			type = type::single_type {type::single_type::class_type {vf_data.cur_cls->class_id}};
		},
		[&](func_call &f) {
			assignable = false;
			if (vf_data.cur_cls != nullptr) {
				auto method = vf_data.cls_map.find_method(vf_data.cur_cls->class_id, f.id);
				if (method != nullptr) {
					verify_call_args(method, f, vf_data, v.base.pos);
					type = method->tp.val;
					return;
				}
			}
			auto func = vf_data.glob_funcs.find_func(f.id);
			if (func == nullptr)
				throw compilation_error(v.base.pos, concat("function '", f.id, "' not found"));
			verify_call_args(func, f, vf_data, v.base.pos);
			type = func->tp.val;
		},
		[&](nested_var::field_ident &i) {
			assignable = true;
			try {
				type = vf_data.vars.get_var_type(i);
			} catch (compilation_error &err) {
				err.error_pos = v.base.pos;
				throw;
			}
		},
		[&](new_val &n) {
			assignable = false;
			if (n.arr_size) {
				auto arr_size_tp = verify_exp_code(*n.arr_size, vf_data);
				if (arr_size_tp != type::single_type {type::single_type::type_enum::INT})
					throw compilation_error(v.base.pos,
						concat("size of array should be of type '",
							type::single_type {type::single_type::type_enum::INT},
							"' not '", arr_size_tp, "'"));
			}
			try {
				auto converted_tp = convert_to_val_type(n.tp);
				verify_type(converted_tp, vf_data);
				type = std::move(converted_tp);
			} catch (compilation_error &err) {
				err.error_pos = v.base.pos;
				throw;
			}
		},
	}, v.base.val);

	// get fields types
	for (auto &field : v.fields) {
		std::visit(overloaded {
			[&](nested_var::self &) { // TODO: maybe just remove this
				assignable = false;
			},
			[&](func_call &f) {
				auto class_tp = try_get_class_tp(type);
				if (!class_tp)
					throw compilation_error(field.pos, concat("cannot call methods from type '", type, "'"));
				auto method = vf_data.cls_map.find_method(class_tp->id, f.id);
				if (method == nullptr)
					throw compilation_error(field.pos, concat("method '", f.id, "' of class '", class_tp->id, "' not found"));
				verify_call_args(method, f, vf_data, field.pos);
				type = method->tp.val;

				if (std::holds_alternative<type::single_type>(type)) {
					auto single_tp = std::get<type::single_type>(type);
					assignable = std::holds_alternative<type::single_type::class_type>(single_tp.val);
				} else if (std::holds_alternative<type::array_type>(type)) {
					assignable = true;
				}
			},
			[&](nested_var::field_ident &i) {
				if (std::holds_alternative<type::array_type>(type)) {
					assignable = false;
					if (i != "length")
						throw compilation_error(field.pos, concat("field name '", i, "' is invalid for type '", type, "'"));
					type = type::single_type {type::single_type::type_enum::INT};
					return;
				}
				assignable = true;
				auto class_tp = try_get_class_tp(type);
				if (!class_tp)
					throw compilation_error(field.pos, concat("cannot access '", i, "' field from type '", type, "'"));
				auto f = vf_data.cls_map.find_field(class_tp->id, i);
				if (f == nullptr)
					throw compilation_error(field.pos, concat("field '", i, "' not found in class '", class_tp->id, "'"));
				type = f->tp.val;
			},
			[&](nested_var::array_pos &a) {
				assignable = true;
				if (!std::holds_alternative<type::array_type>(type))
					throw compilation_error(field.pos, concat("type '", type, "' is not an array"));
				auto array_tp = std::get<type::array_type>(type);
				auto pos_tp = verify_exp_code(a, vf_data);
				if (pos_tp != type::single_type {type::single_type::type_enum::INT})
					throw compilation_error(field.pos, concat("array pos needs to be of type '",
						type::single_type::type_enum::INT, "', not '", pos_tp, "'"));
				type = array_tp.val;
			},
		}, field.val);
	}
	return get_nested_var_type_return {
		.tp = std::move(type),
		.assignable = assignable
	};
}

type::val_type verify_exp_code(exp::uptr &ex, verifier_data &vf_data) {
	auto ret = visit(overloaded {
		[&](nested_var &e) -> type::val_type {
			return get_nested_var_type(e, vf_data).tp;
		},
		[&](exp::binary &e) -> type::val_type {
			auto exp1_tp = verify_exp_code(e.e1, vf_data);
			auto exp2_tp = verify_exp_code(e.e2, vf_data);
			if (!can_convert_type(exp1_tp, exp2_tp, vf_data) && !can_convert_type(exp2_tp, exp1_tp, vf_data))
				throw compilation_error(ex->pos, concat("invalid type of arguments in binary expression '",
					exp1_tp, "' and '", exp2_tp, "'"));
			bool is_eq_neq = is_same_oper(e.op, exp::binary::rel_op_type::EQU) ||
				is_same_oper(e.op, exp::binary::rel_op_type::NE);
			bool is_add = is_same_oper(e.op, exp::binary::arithmetic_op_type::ADD);
			auto single_tp = std::visit(overloaded {
				[&e, is_eq_neq, is_add](type::single_type &tp) {
					return std::visit(overloaded {
						[&e, is_eq_neq, is_add](type::single_type::type_enum &tp) {
							switch (tp) {
							case type::single_type::type_enum::INT:
								if (is_arithmetic_oper(e.op))
									return type::single_type::type_enum::INT;
								if (is_rel_oper(e.op))
									return type::single_type::type_enum::BOOL;
								break;
							case type::single_type::type_enum::BOOL:
								if (is_bool_oper(e.op) || is_eq_neq)
									return type::single_type::type_enum::BOOL;
								break;
							case type::single_type::type_enum::STR:
								if (is_add)
									return type::single_type::type_enum::STR;
								if (is_eq_neq)
									return type::single_type::type_enum::BOOL;
								break;
							case type::single_type::type_enum::VOID:
								throw compilation_error(e.e1->pos, "void cannot occur in binary expression");
							}
							throw compilation_error(e.e1->pos, concat("invalid operator '", e.op,
								"' in binary expression with expressions of type '", tp, "'"));
						},
						[&e, is_eq_neq](type::single_type::class_type &tp) {
							if (is_eq_neq)
								return type::single_type::type_enum::BOOL;
							throw compilation_error(e.e1->pos, concat("invalid operator '", e.op,
								"' in binary expression with expressions of type '", tp, "'"));
						},
						[&e, is_eq_neq](type::single_type::null_type &tp) {
							if (is_eq_neq)
								return type::single_type::type_enum::BOOL;
							throw compilation_error(e.e1->pos, concat("invalid operator '", e.op,
								"' in binary expression with expressions of type '", tp, "'"));
						}
					}, tp.val);
				},
				[&e, is_eq_neq](type::array_type &tp) {
					if (is_eq_neq)
						return type::single_type::type_enum::BOOL;
					throw compilation_error(e.e1->pos, concat("invalid operator '", e.op,
						"' in binary expression with expressions of type '", tp, "'"));
				},
			}, exp1_tp);
			return type::single_type {single_tp};
		},
		[&](exp::unary &e) -> type::val_type {
			auto exp_tp = verify_exp_code(e.e, vf_data);
			type::val_type expected_type;
			switch (e.op) {
			case exp::unary::op_type::NOT:
				expected_type = type::single_type {type::single_type::type_enum::BOOL};
				break;
			case exp::unary::op_type::NEG:
				expected_type = type::single_type {type::single_type::type_enum::INT};
				break;
			}
			if (exp_tp != expected_type)
				throw compilation_error(e.e->pos,
					concat("invalid type of expression for unary operation - expected '",
						expected_type, "' instead of '", exp_tp, "'"));
			return exp_tp;
		},
		[&](exp::null_casted &e) -> type::val_type {
			return convert_to_val_type(e.tp);
		},
		[&](exp::null &) -> type::val_type {
			return type::single_type {type::single_type::null_type {}};
		},
		[&](int &) -> type::val_type {
			return type::single_type {type::single_type::type_enum::INT};
		},
		[&](bool &) -> type::val_type {
			return type::single_type {type::single_type::type_enum::BOOL};
		},
		[&](std::string &) -> type::val_type {
			return type::single_type {type::single_type::type_enum::STR};
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

void verify_block_code(block::uptr &bl, verifier_data &vf_data, bool push_new_block = true);

void verify_statement_code(stmt::uptr &st, verifier_data &vf_data) {
	auto get_assignable_nested_var_type = [&vf_data](nested_var &v, tree_node *node) {
		try {
			auto [var_tp, assignable] = get_nested_var_type(v, vf_data);
			if (!assignable)
				throw compilation_error("tried to assign to non-assignable value");
			return var_tp;
		} catch (compilation_error &err) {
			err.error_pos = node->pos;
			throw;
		}
	};
	visit(overloaded {
		[&](stmt::block &bl) {
			verify_block_code(bl.bl, vf_data);
		},
		[&](stmt::decl &s) {
			try {
				verify_type(s.tp.val, vf_data);
			} catch (compilation_error &err) {
				err.error_pos = st->pos;
				throw;
			}
			for (auto &item : s.items) {
				if (item->init_val) {
					auto exp_tp = verify_exp_code(*item->init_val, vf_data);
					if (!can_convert_type(exp_tp, s.tp.val, vf_data))
						throw compilation_error((*item->init_val)->pos,
							concat("cannot initialize a variable of type '", s.tp.val,
								"' with expression of type '", exp_tp, "'"));
				}
				try {
					vf_data.vars.push_var(item->id, s.tp.val);
				} catch (compilation_error &err) {
					err.error_pos = item->pos;
					throw;
				}
			}
		},
		[&](stmt::ass &s) {
			auto var_tp = get_assignable_nested_var_type(s.v, st.get());
			auto exp_tp = verify_exp_code(s.e, vf_data);
			if (!can_convert_type(exp_tp, var_tp, vf_data))
				throw compilation_error(s.e->pos, concat("cannot assign expression of type '", exp_tp,
					"' to variable of type '", var_tp, "'"));
		},
		[&](stmt::unary &s) {
			auto var_tp = get_assignable_nested_var_type(s.v, st.get());
			if (var_tp != type::single_type {type::single_type::type_enum::INT})
				throw compilation_error(st->pos,
					concat("invalid type of '", s.v, "' for this operation - should be 'int'"));
		},
		[&](stmt::ret &s) {
			if (s.e) {
				auto exp_tp = verify_exp_code(*s.e, vf_data);
				if (!can_convert_type(exp_tp, vf_data.cur_fun->tp.val, vf_data))
					throw compilation_error(st->pos,
						concat("invalid type of return expression '", exp_tp,
							"' - should be '", vf_data.cur_fun->tp.val, "'"));
			} else {
				if (vf_data.cur_fun->tp.val != type::single_type {type::single_type::type_enum::VOID})
					throw compilation_error(st->pos,
						concat("invalid type of return expression '",
							type::single_type {type::single_type::type_enum::VOID},
							"' - should be '", vf_data.cur_fun->tp, "'"));
			}
		},
		[&](stmt::cond &s) {
			auto exp_tp = verify_exp_code(s.e, vf_data);
			if (exp_tp != type::single_type {type::single_type::type_enum::BOOL})
				throw compilation_error(s.e->pos, concat("invalid type of condition expression '",
					exp_tp, "' - should be 'boolean'"));
			ensure_new_block(s.s_true);
			verify_statement_code(s.s_true, vf_data);
			if (s.s_false) {
				ensure_new_block(*s.s_false);
				verify_statement_code(*s.s_false, vf_data);
			}
		},
		[&](stmt::loop_while &s) {
			auto exp_tp = verify_exp_code(s.e, vf_data);
			if (exp_tp != type::single_type {type::single_type::type_enum::BOOL})
				throw compilation_error(s.e->pos, concat("invalid type of condition expression '",
					exp_tp, "' - should be 'boolean'"));
			ensure_new_block(s.s);
			verify_statement_code(s.s, vf_data);
		},
		[&](stmt::loop_for &s) {
			try {
				verify_single_type(s.tp, vf_data);
			} catch (compilation_error &err) {
				err.error_pos = st->pos;
				throw;
			}
			auto nest_tp = get_nested_var_type(s.v, vf_data).tp;
			if (!std::holds_alternative<type::array_type>(nest_tp))
				throw compilation_error(s.v.base.pos,
					concat("for loop available only over array type, here '", nest_tp, "'"));
			auto array_tp = std::get<type::array_type>(nest_tp);
			if (!can_convert_type(array_tp.val, type::val_type {s.tp}, vf_data))
				throw compilation_error(st->pos, concat("cannot assign expression of type '", array_tp.val,
					"' to variable of type '", s.tp, "'"));
			ensure_new_block(s.s);
			vf_data.vars.push_block();
			vf_data.vars.push_var(s.id, s.tp);
			verify_statement_code(s.s, vf_data);
			vf_data.vars.pop_block();
		},
		[&](stmt::exp &s) {
			verify_exp_code(s.e, vf_data);
		},
		[&](stmt::empty &) {},
	}, st->val);
}

void verify_block_code(block::uptr &bl, verifier_data &vf_data, bool push_new_block)
{
	if (push_new_block)
		vf_data.vars.push_block();
	for (auto &stmt : bl->stmts)
		verify_statement_code(stmt, vf_data);
	if (push_new_block)
		vf_data.vars.pop_block();
}

void verify_function(func::uptr &f, verifier_data &vf_data) {
	vf_data.vars.push_block();
	for (auto &arg : f->args) {
		try {
			verify_type(arg.tp.val, vf_data);
			vf_data.vars.push_var(arg.id, arg.tp.val);
		} catch (compilation_error &err) {
			err.error_pos = arg.pos;
			throw;
		}
	}
	try {
		verify_type(f->tp.val, vf_data);
	} catch (compilation_error &err) {
		err.error_pos = f->pos;
		throw;
	}
	vf_data.cur_fun = f.get();
	verify_block_code(f->bl, vf_data, false);
	vf_data.cur_fun = nullptr;
	vf_data.vars.pop_block();
}

void verify_class(class_def::uptr &c, verifier_data &vf_data) {
	vf_data.vars.push_block();
	for (auto &[field_id, field] : vf_data.cls_map.get_class_fields_map(c->class_id)) {
		try {
			verify_type(field->tp.val, vf_data);
			vf_data.vars.push_var(field_id, field->tp.val);
		} catch (compilation_error &err) {
			err.error_pos = field->pos;
			throw;
		}
	}
	vf_data.cur_cls = c.get();
	for (auto &def : c->defs) {
		if (!std::holds_alternative<func::uptr>(def))
			continue;
		auto &f = std::get<func::uptr>(def);
		verify_function(f, vf_data);
	}
	vf_data.cur_cls = nullptr;
	vf_data.vars.pop_block();
}

void verify_program(program::uptr &prog) {
	verifier_data vf_data;
	prepare_verifier_data(prog, vf_data);
	for (auto &def : prog->defs) {
		std::visit(overloaded {
			[&](func::uptr &f) {
				verify_function(f, vf_data);
			},
			[&](class_def::uptr &c) {
				verify_class(c, vf_data);
			}
		}, def);
	}
}

} // namespace latte::frontend
