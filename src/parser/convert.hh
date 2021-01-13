#pragma once

extern "C" {
#include "Absyn.h"
}

#include "core/tree_types.hh"

#include <string>

namespace latte::parser {

core::program::uptr convert_program(Program p);

} // latte::parser
