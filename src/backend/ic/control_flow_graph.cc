#include "backend/ic/control_flow_graph.hh"
#include "utils/utils.hh"

#include <iostream>
#include <map>
#include <set>
#include <variant>

namespace latte::backend {

void update_kill_use_info(const instr &i, std::function<void(const core::ident &)> kill,
		std::function<void(const core::ident &)> use) {
	auto use_if_var = [&](const instr::arg &a) {
		if (std::holds_alternative<instr::arg::var>(a.val))
			use(std::get<instr::arg::var>(a.val).id);
	};
	std::visit(overloaded {
		[&](instr::prepare_call_arg i) {
			use_if_var(i.a);
		},
		[&](instr::call_ass i) {
			kill(i.res_id);
		},
		[&](instr::bin_ass i) {
			kill(i.id);
			use_if_var(i.a1);
			use_if_var(i.a2);
		},
		[&](instr::unary_ass i) {
			kill(i.id);
			use(i.v.id);
		},
		[&](instr::set i) {
			kill(i.id);
			use_if_var(i.a);
		},
		[&](instr::cond_jump i) {
			use_if_var(i.a1);
			use_if_var(i.a2);
		},
		[&](instr::ret i) {
			if (i.a)
				use_if_var(*i.a);
		},
		[&](auto) {},
	}, i.val);
}

basic_block::basic_block(std::vector<instr> instrs) {
	auto kill = [&](const core::ident &id) {
		_kill.emplace(id);
		_use.erase(id);
	};
	auto use = [&](const core::ident &id) {
		_use.emplace(id);
		_kill.erase(id);
	};

	for (ssize_t i = instrs.size() - 1; i >= 0; --i)
		update_kill_use_info(instrs[i], kill, use);

	for (auto &i : instrs)
		_instrs.push_back({std::move(i), {}});
}

void basic_block::calc_alive_after() {
	auto cur_alive = _end_alive;
	auto kill = [&](const core::ident &id) {
		cur_alive.erase(id);
	};
	auto use = [&](const core::ident &id) {
		cur_alive.emplace(id);
	};
	for (ssize_t i = _instrs.size() - 1; i >= 0; --i) {
		_instrs[i].alive_after = cur_alive;
		update_kill_use_info(_instrs[i].i, kill, use);
	}
}

std::vector<basic_block> control_flow_graph::create_basic_blocks(const std::vector<instr> &instrs) {
	std::set<std::string> active_labels;
	for (auto &i : instrs) {
		std::visit(overloaded {
			[&](instr::jump j) {
				active_labels.emplace(j.label);
			},
			[&](instr::cond_jump j) {
				active_labels.emplace(j.label_true);
			},
			[&](auto) {},
		}, i.val);
	}

	std::vector<basic_block> blocks;
	std::vector<instr> block_instrs;
	auto next_block = [&]() {
		blocks.emplace_back(std::move(block_instrs));
		block_instrs.clear();
	};
	auto is_active_label = [&](const instr &i) {
		if (!std::holds_alternative<instr::label>(i.val))
			return false;
		auto &label = std::get<instr::label>(i.val);
		return active_labels.find(label.name) != active_labels.end();
	};
	auto is_jump = [](const instr &i) {
		return std::holds_alternative<instr::jump>(i.val) || std::holds_alternative<instr::cond_jump>(i.val);
	};
	for (auto &i : instrs) {
		if (is_active_label(i) && !block_instrs.empty()) {
			next_block();
		}
		block_instrs.emplace_back(i);
		if (is_jump(i))
			next_block();
	}
	if (!block_instrs.empty())
		next_block();

	std::map<std::string, basic_block*> name_to_block;
	for (auto &b : blocks)
		if (std::holds_alternative<instr::label>(b._instrs[0].i.val))
			name_to_block[std::get<instr::label>(b._instrs[0].i.val).name] = &b;

	for (size_t i = 0; i < blocks.size(); ++i) {
		auto &b = blocks[i];
		auto is_jump = std::holds_alternative<instr::jump>(b._instrs.back().i.val);
		auto is_cond_jump = std::holds_alternative<instr::cond_jump>(b._instrs.back().i.val);
		auto is_return = std::holds_alternative<instr::ret>(b._instrs.back().i.val);
		std::optional<std::string> jump_name;
		if (is_jump)
			jump_name = std::get<instr::jump>(b._instrs.back().i.val).label;
		else if (is_cond_jump)
			jump_name = std::get<instr::cond_jump>(b._instrs.back().i.val).label_true;

		if (jump_name) {
			auto next_b = name_to_block[*jump_name];
			b._next_blocks.emplace_back(next_b);
			next_b->_prev_blocks.emplace_back(&b);
		}

		if (!is_jump && !is_return && i + 1 != blocks.size()) {
			b._next_blocks.emplace_back(&blocks[i + 1]);
			blocks[i + 1]._prev_blocks.emplace_back(&b);
		}
	}

	return blocks;
}

control_flow_graph::control_flow_graph(const func &f)
		: _blocks(create_basic_blocks(f.instrs)), _meta(f.meta) {
	auto add_set = [](std::set<core::ident> &dst, const std::set<core::ident> &src) {
		for (auto &i : src)
			dst.emplace(i);
	};
	auto sub_set = [](std::set<core::ident> &dst, const std::set<core::ident> &src) {
		for (auto &i : src)
			dst.erase(i);
	};

	// calculate Ins
	while (true) {
		bool rep = false;
		for (auto &b : _blocks) {
			auto prev = b._beg_alive;
			for (auto *nb : b._next_blocks)
				add_set(b._beg_alive, nb->_beg_alive);
			sub_set(b._beg_alive, b._kill);
			add_set(b._beg_alive, b._use);
			if (prev != b._beg_alive)
				rep = true;
		}
		if (!rep)
			break;
	}

	// calculate Outs
	for (auto &b : _blocks)
		for (auto *nb : b._next_blocks)
			add_set(b._end_alive, nb->_beg_alive);

	for (auto &b : _blocks)
		b.calc_alive_after();
}

} // namespace latte::backend
