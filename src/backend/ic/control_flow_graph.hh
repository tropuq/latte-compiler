#pragma once

#include "backend/ic/code_types.hh"

#include <vector>
#include <set>

namespace latte::backend {

class basic_block {
public:
	struct instr_info {
		instr i;
		std::set<core::ident> alive_after;
	};
private:
	std::set<core::ident> _kill;
	std::set<core::ident> _use;

	std::set<core::ident> _beg_alive;
	std::set<core::ident> _end_alive;

	std::vector<instr_info> _instrs;
	std::vector<basic_block*> _prev_blocks;
	std::vector<basic_block*> _next_blocks;

	friend class control_flow_graph;

	void calc_alive_after();
public:
	basic_block(std::vector<instr> instrs);
	const std::set<core::ident> &get_beg_alive() const {
		return _beg_alive;
	}
	const std::set<core::ident> &get_end_alive() const {
		return _end_alive;
	}
	const std::vector<instr_info> &get_instr_info() const {
		return _instrs;
	}
	std::ostream &print(std::ostream &ost) const;
};

class control_flow_graph {
	std::vector<basic_block> _blocks;
	func::metadata _meta;
	static std::vector<basic_block> create_basic_blocks(const std::vector<instr> &instrs);
public:
	control_flow_graph(const func &f);
	const std::vector<basic_block> &blocks() const {
		return _blocks;
	}
	const func::metadata &metadata() const {
		return _meta;
	}
	std::ostream &print(std::ostream &ost) const;
};

class program_graph {
	std::vector<vtable_description> _vtbl;
	std::vector<control_flow_graph> _cfgs;
public:
	program_graph(const program &prog);
	const std::vector<control_flow_graph> &control_flow_graphs() const {
		return _cfgs;
	}
	const std::vector<vtable_description> &vtables() const {
		return _vtbl;
	}
	std::ostream &print(std::ostream &ost) const;
};

} // namespace latte::backend

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::basic_block &bl) {
	return bl.print(ost);
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::control_flow_graph &cfg) {
	return cfg.print(ost);
}

inline std::ostream &operator<<(std::ostream &ost, const latte::backend::program_graph &p) {
	return p.print(ost);
}
