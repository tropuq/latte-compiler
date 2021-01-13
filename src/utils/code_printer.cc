#include "code_printer.hh"
#include "colors.hh"
#include "utils.hh"
#include <fstream>
#include <optional>
#include <stdexcept>

namespace latte {

code_printer::code_printer(std::string filename)
		: _filename(std::move(filename)) {
	std::ifstream f(_filename);
	while (f) {
		std::string line;
		std::getline(f, line);
		_code_lines.emplace_back(std::move(line));
	}
}

std::ostream &code_printer::print_line(std::ostream &ost, const std::string &msg,
		size_t line_pos, std::optional<size_t> char_pos) const {
	if (line_pos == 0 || line_pos >= _code_lines.size())
		return print_msg(ost, msg);

	auto &line = _code_lines[line_pos - 1];
	if (char_pos && *char_pos >= line.size())
		*char_pos = line.size();

	ost << colors::white_bold << _filename << ':' << line_pos;
	if (char_pos)
		ost << ':' << *char_pos;
	ost << ": " << colors::reset << msg << '\n' << line << '\n';
	if (char_pos && *char_pos > 0) {
		auto arrow_line = line;
		for (char &c : arrow_line)
			if (!isspace(c))
				c = ' ';
		arrow_line[*char_pos - 1] = '^';
		ost << arrow_line << '\n';
	}
	return ost;
}

std::ostream &code_printer::print_msg(std::ostream &ost, const std::string &msg) const {
	return ost << colors::white_bold << _filename << ": " << colors::reset << msg << '\n';
}

} // namespace latte
