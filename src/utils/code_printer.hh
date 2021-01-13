#pragma once

#include <vector>
#include <string>
#include <optional>

namespace latte {

class code_printer {
	std::vector<std::string> _code_lines;
	std::string _filename;
public:
	code_printer(std::string filename);

	std::ostream &print_line(std::ostream &ost, const std::string &msg,
		size_t line_pos, std::optional<size_t> char_pos = std::nullopt) const;

	std::ostream &print_msg(std::ostream &ost, const std::string &msg) const;
};

} // namespace latte
