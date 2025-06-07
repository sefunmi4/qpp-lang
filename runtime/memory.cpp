#include "memory.h"

namespace qpp {
int MemoryManager::create_qregister(size_t n) {
    qregs.emplace_back(n);
    return static_cast<int>(qregs.size() - 1);
}

int MemoryManager::create_cregister(size_t n) {
    cregs.emplace_back(n);
    return static_cast<int>(cregs.size() - 1);
}

QRegister& MemoryManager::qreg(int id) {
    return qregs.at(id);
}

CRegister& MemoryManager::creg(int id) {
    return cregs.at(id);
}

MemoryManager memory;
} // namespace qpp

