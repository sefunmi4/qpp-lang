#pragma once
#include <vector>
#include <string>
#include "memory.h"

namespace qpp {
// Replace sequences of instructions with optimized stubs.
void optimize_patterns(std::vector<std::vector<std::string>>& ops);

// Direct pattern helpers used by the runtime
void apply_qft2(QRegister& qr, std::size_t q0, std::size_t q1);
void apply_grover2(QRegister& qr, std::size_t q0, std::size_t q1);
}
