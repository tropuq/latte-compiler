#pragma once

#include <cstdlib>

namespace latte::core {

struct code_pos {
	size_t line_pos;
	size_t char_pos;
};

inline code_pos convert_bnfc_pos(int line_pos, int char_pos) {
	return code_pos {
		.line_pos = (size_t)line_pos,
		.char_pos = (size_t)char_pos + (line_pos != 1),
	};
}

} // namespace latte::core
