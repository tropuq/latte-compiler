#include "lcse.hh"
#include "utils/utils.hh"

#include <map>
#include <tuple>
#include <variant>

namespace latte::backend {

using variable_id = instr::variable_id;
using address_offset = instr::address_offset;
using binary_op = core::exp::binary::arithmetic_op_type;
using unary_op = core::exp::unary::op_type;

using component = std::variant<variable_id, int, bool, std::string>;
using component_set = std::set<component>;
using variable_set = std::set<variable_id>;

bool operator<(const variable_id &l, const variable_id &r) {
	return l.id < r.id;
}

bool operator<(const component &a, const component &b) {
	auto get_component_tier = [](const component &c) {
		return std::visit(overloaded {
			[](const variable_id &) {
				return 4;
			},
			[](const int &) {
				return 3;
			},
			[](const bool &) {
				return 2;
			},
			[](const std::string &) {
				return 1;
			},
		}, c);
	};
	auto a_tier = get_component_tier(a);
	auto b_tier = get_component_tier(b);
	if (a_tier != b_tier)
		return a_tier < b_tier;
	return std::visit(overloaded {
		[](const variable_id &l, const variable_id &r) {
			return l < r;
		},
		[](const int &l, const int &r) {
			return l < r;
		},
		[](const bool &l, const bool &r) {
			return l < r;
		},
		[](const std::string &l, const std::string &r) {
			return l < r;
		},
		[](auto &, auto &) {
			assert(false);
			return false;
		}
	}, a, b);
}

bool is_var(const component &r) {
	return std::holds_alternative<variable_id>(r);
}

variable_id get_var(const component &r) {
	return std::get<variable_id>(r);
}

struct binary_assign_rhs {
	component_set right1;
	component_set right2;
	binary_op op;
};

struct unary_assign_rhs {
	component_set right;
	unary_op op;
};

class assignment_manager {
	std::vector<std::pair<binary_assign_rhs, variable_set>> _active_binary_ass;
	std::vector<std::pair<unary_assign_rhs, variable_set>> _active_unary_ass;
	std::vector<component_set> _equal_vars;

	component_set get_component_set(component v) {
		auto it = std::find_if(_equal_vars.begin(), _equal_vars.end(),
			[&](component_set &s) {
				return s.count(v);
			});
		if (it != _equal_vars.end())
			return *it;
		return {v};
	}

	void do_add_binary_if_valid(binary_assign_rhs bin_ass, variable_set l) {
		if (bin_ass.right1.empty() || bin_ass.right2.empty())
			return;
		_active_binary_ass.emplace_back(std::move(bin_ass), std::move(l));
	}

	void do_add_unary_if_valid(unary_assign_rhs un_ass, variable_set l) {
		if (un_ass.right.empty())
			return;
		_active_unary_ass.emplace_back(std::move(un_ass), std::move(l));
	}

public:
	component get_least_tier_equal_component(const component &v) const {
		auto it = std::find_if(_equal_vars.begin(), _equal_vars.end(),
			[&](const component_set &s) {
				return s.count(v);
			});
		if (it != _equal_vars.end())
			return *it->begin();
		return v;
	}

	void modify_var(variable_id v) {
		for (auto &s : _equal_vars)
			s.erase(v);
		for (auto &u : _active_unary_ass) {
			u.first.right.erase(v);
			u.second.erase(v);
		}
		for (auto &b : _active_binary_ass) {
			b.first.right1.erase(v);
			b.first.right2.erase(v);
			b.second.erase(v);
		}
		_equal_vars.erase(
			std::remove_if(_equal_vars.begin(), _equal_vars.end(), [](component_set &s) {
				return s.size() <= 1;
			}), _equal_vars.end());
		_active_unary_ass.erase(
			std::remove_if(_active_unary_ass.begin(), _active_unary_ass.end(), [](std::pair<unary_assign_rhs, variable_set> &s) {
				return s.first.right.empty() || s.second.empty();
			}), _active_unary_ass.end());
		_active_binary_ass.erase(
			std::remove_if(_active_binary_ass.begin(), _active_binary_ass.end(), [](std::pair<binary_assign_rhs, variable_set> &s) {
				return s.first.right1.empty() || s.first.right2.empty() || s.second.empty();
			}), _active_binary_ass.end());
	}

	void add_ass(variable_id l, component r) {
		modify_var(l);
		bool added = false;
		for (auto &s : _equal_vars) {
			if (s.count(r)) {
				s.emplace(l);
				added = true;
				break;
			}
		}
		if (!added) {
			_equal_vars.emplace_back();
			_equal_vars.back().emplace(l);
			_equal_vars.back().emplace(r);
		}
		for (auto &u : _active_unary_ass) {
			if (u.first.right.count(r))
				u.first.right.emplace(l);
			if (is_var(r) && u.second.count(get_var(r)))
				u.second.emplace(l);
		}
		for (auto &b : _active_binary_ass) {
			if (b.first.right1.count(r))
				b.first.right1.emplace(l);
			if (b.first.right2.count(r))
				b.first.right2.emplace(l);
			if (is_var(r) && b.second.count(get_var(r)))
				b.second.emplace(l);
		}
	}

	std::optional<component> get_binary_ass(component right1, component right2, binary_op op) {
		auto it = std::find_if(_active_binary_ass.begin(), _active_binary_ass.end(),
			[&](std::pair<binary_assign_rhs, variable_set> &s) {
				return s.first.op == op && s.first.right1.count(right1) && s.first.right2.count(right2);
			});
		if (it != _active_binary_ass.end())
			return *it->second.begin();
		return std::nullopt;
	}

	std::optional<component> get_unary_ass(component right, unary_op op) {
		auto it = std::find_if(_active_unary_ass.begin(), _active_unary_ass.end(),
			[&](std::pair<unary_assign_rhs, variable_set> &s) {
				return s.first.op == op && s.first.right.count(right);
			});
		if (it != _active_unary_ass.end())
			return *it->second.begin();
		return std::nullopt;

	}

	std::optional<component> add_binary_ass(variable_id l, component right1, component right2, binary_op op) {
		auto bin_ass = get_binary_ass(right1, right2, op);
		if (bin_ass) {
			add_ass(l, *bin_ass);
			return *bin_ass;
		}
		auto rs1 = get_component_set(right1);
		auto rs2 = get_component_set(right2);
		modify_var(l);
		rs1.erase(l);
		rs2.erase(l);
		variable_set ls = {l};
		do_add_binary_if_valid(binary_assign_rhs {std::move(rs1), std::move(rs2), op}, std::move(ls));
		return std::nullopt;
	}

	std::optional<component> add_unary_ass(variable_id l, component right, unary_op op) {
		auto un_ass = get_unary_ass(right, op);
		if (un_ass) {
			add_ass(l, *un_ass);
			return *un_ass;
		}
		auto rs = get_component_set(right);
		modify_var(l);
		rs.erase(l);
		variable_set ls = {l};
		do_add_unary_if_valid(unary_assign_rhs {std::move(rs), op}, std::move(ls));
		return std::nullopt;
	}

	std::ostream &print(std::ostream &ost) const {
		ost << "Equal sets:\n";
		for (auto &i : _equal_vars)
			ost << "   {" << i << "}\n";
		ost << "Unary assignments:\n";
		for (auto &i : _active_unary_ass)
			ost << "   " << i.first.op << "{" << i.first.right << "} -> {" << i.second << "}\n";
		ost << "Binary assignments:\n";
		for (auto &i : _active_binary_ass)
			ost << "   {" << i.first.right1 << "} " << i.first.op << " {" << i.first.right2 << "} -> {" << i.second << "}\n";
		return ost;
	}
};

} // namespace latte::backend

std::ostream &operator<<(std::ostream &ost, const latte::backend::assignment_manager &mgmt) {
	return mgmt.print(ost);
}

namespace latte::backend {

instr::rvalue::val_type component_to_rvalue_val_type(component c) {
	return std::visit([](auto &v) -> instr::rvalue::val_type {
			return std::move(v);
		}, c);
}

void perform_lcse(std::vector<instr> &instrs) {
	assignment_manager mgmt;
	auto rvalue_to_component = [](const instr::rvalue &v) -> std::optional<component> {
		std::optional<component> comp = std::nullopt;
		std::visit(overloaded {
			[&](const variable_id &v) {
				comp = v;
			},
			[&](const int &v) {
				comp = v;
			},
			[&](const bool &v) {
				comp = v;
			},
			[&](const std::string &v) {
				comp = v;
			},
			[](auto &) {}
		}, v.val);
		return comp;
	};
	auto rvalue_to_variable_id = [](auto &v) {
		return std::holds_alternative<variable_id>(v.val) ?
			std::make_optional(std::get<variable_id>(v.val)) : std::nullopt;
	};
	for (auto &instr : instrs) {
		auto substitute_to_set = [&](instr::lvalue lv, std::optional<component> new_var) {
			if (!new_var)
				return;
			auto tp = lv.tp;
			instr.val = instr::set {
				.lv = std::move(lv),
				.rv = {component_to_rvalue_val_type(std::move(*new_var)), tp}
			};
		};
		auto simplify_rvalue = [&](instr::rvalue &rv) {
			auto var = rvalue_to_variable_id(rv);
			if (!var)
				return;
			auto eq_comp = mgmt.get_least_tier_equal_component(*var);
			rv.val = component_to_rvalue_val_type(std::move(eq_comp));
		};
		std::visit(overloaded {
			[&](instr::prepare_call_arg &i) {
				simplify_rvalue(i.rv);
			},
			[&](instr::ret &i) {
				if (i.val)
					simplify_rvalue(*i.val);
			},
			[&](instr::call_ass &i) {
				auto lv = rvalue_to_variable_id(i.lv);
				if (lv)
					mgmt.modify_var(*lv);
			},
			[&](instr::bin_ass &i) {
				simplify_rvalue(i.rv1);
				simplify_rvalue(i.rv2);
				auto lv = rvalue_to_variable_id(i.lv);
				auto rv1 = rvalue_to_component(i.rv1);
				auto rv2 = rvalue_to_component(i.rv2);
				if (lv && rv1 && rv2)
					substitute_to_set(i.lv, mgmt.add_binary_ass(*lv, *rv1, *rv2, i.op));
				else if (rv1 && rv2)
					substitute_to_set(i.lv, mgmt.get_binary_ass(*rv1, *rv2, i.op));
				else if (lv)
					mgmt.modify_var(*lv);
			},
			[&](instr::unary_ass &i) {
				simplify_rvalue(i.rv);
				auto lv = rvalue_to_variable_id(i.lv);
				auto rv = rvalue_to_component(i.rv);
				if (lv && rv)
						substitute_to_set(i.lv, mgmt.add_unary_ass(*lv, *rv, i.op));
				else if (rv)
					substitute_to_set(i.lv, mgmt.get_unary_ass(*rv, i.op));
				else if (lv)
					mgmt.modify_var(*lv);
			},
			[&](instr::set &i) {
				simplify_rvalue(i.rv);
				auto lv = rvalue_to_variable_id(i.lv);
				auto rv = rvalue_to_component(i.rv);
				if (lv && rv)
					mgmt.add_ass(*lv, *rv);
				else if (lv)
					mgmt.modify_var(*lv);
			},
			[&](auto &) {}
		}, instr.val);
	}
}

} // namespace latte::backend
