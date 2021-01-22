#pragma once

#include "asm_code.hh"
#include "backend/ic/control_flow_graph.hh"

#include <vector>

namespace latte::backend {

std::vector<asm_code> gen_asm_for_prog(const program_graph &p);

}
