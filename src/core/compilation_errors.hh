#pragma once

#include "code_pos.hh"
#include "utils/code_printer.hh"
#include "utils/colors.hh"
#include "utils/utils.hh"

#include <ostream>
#include <stdexcept>
#include <optional>

namespace latte::core {

class compilation_error : public std::runtime_error {
public:
	std::optional<code_pos> error_pos;

	compilation_error(code_pos pos, std::string msg)
		: std::runtime_error(std::move(msg)), error_pos(pos) {}
	compilation_error(std::string msg) : std::runtime_error(msg) {}

	std::ostream &print_error(std::ostream &ost, code_printer &printer) {
		std::string error_msg(concat(colors::red_bold, "error: ", colors::reset, what()));
		if (error_pos)
			return printer.print_line(ost, error_msg, error_pos->line_pos, error_pos->char_pos);
		return printer.print_msg(ost, error_msg);
	}
};


} // namespace latte::core
