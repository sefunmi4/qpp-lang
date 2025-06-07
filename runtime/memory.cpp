#include "memory.h"
#include <stdexcept>

namespace qpp {
int MemoryManager::create_qregister(size_t n) {
    qregs.push_back(std::make_unique<QRegister>(n));
    return static_cast<int>(qregs.size() - 1);
}

bool MemoryManager::release_qregister(int id) {
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        return false;
    qregs[id].reset();
    return true;
}

int MemoryManager::create_cregister(size_t n) {
    cregs.push_back(std::make_unique<CRegister>(n));
    return static_cast<int>(cregs.size() - 1);
}

bool MemoryManager::release_cregister(int id) {
    if (id < 0 || id >= static_cast<int>(cregs.size()) || !cregs[id])
        return false;
    cregs[id].reset();
    return true;
}

QRegister& MemoryManager::qreg(int id) {
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        throw std::out_of_range("invalid qregister id");
    return *qregs[id];
}

CRegister& MemoryManager::creg(int id) {
    if (id < 0 || id >= static_cast<int>(cregs.size()) || !cregs[id])
        throw std::out_of_range("invalid cregister id");
    return *cregs[id];

}

MemoryManager memory;
} // namespace qpp

