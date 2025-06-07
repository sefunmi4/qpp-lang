#include "memory.h"
#include <stdexcept>
#include <mutex>

namespace qpp {
// TODO(good-first-issue): track register usage statistics for debugging

int MemoryManager::create_qregister(size_t n) {
    std::lock_guard<std::mutex> lock(mtx);
    qregs.push_back(std::make_unique<QRegister>(n));
    return static_cast<int>(qregs.size() - 1);
}

bool MemoryManager::release_qregister(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        return false;
    qregs[id].reset();
    return true;
}

int MemoryManager::create_cregister(size_t n) {
    std::lock_guard<std::mutex> lock(mtx);
    cregs.push_back(std::make_unique<CRegister>(n));
    return static_cast<int>(cregs.size() - 1);
}

bool MemoryManager::release_cregister(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(cregs.size()) || !cregs[id])
        return false;
    cregs[id].reset();
    return true;
}

QRegister& MemoryManager::qreg(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        throw std::out_of_range("invalid qregister id");
    return *qregs[id];
}

CRegister& MemoryManager::creg(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(cregs.size()) || !cregs[id])
        throw std::out_of_range("invalid cregister id");
    return *cregs[id];
}

MemoryManager memory;
// TODO(good-first-issue): expose import/export of register states for persistence
} // namespace qpp


