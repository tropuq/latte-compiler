#pragma once

#include "asm_code.hh"

namespace latte::backend {

void remove_dead_code(std::vector<asm_code> &code, const std::string &epilog_label);

} // namespace latte::backend
