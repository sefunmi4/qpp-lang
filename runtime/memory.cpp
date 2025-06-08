#include "memory.h"
#include <stdexcept>
#include <mutex>
#include <fstream>

namespace qpp {
int MemoryManager::create_qregister(size_t n) {
    std::lock_guard<std::mutex> lock(mtx);
    int id;
    if (!free_qids.empty()) {
        id = free_qids.back();
        free_qids.pop_back();
        if (id >= static_cast<int>(qregs.size())) {
            qregs.resize(id + 1);
        }
        qregs[id] = std::make_unique<QRegister>(n);
        if (id >= static_cast<int>(qalloc_count.size()))
            qalloc_count.resize(id + 1, 0);
        ++qalloc_count[id];
    } else {
        id = static_cast<int>(qregs.size());
        qregs.push_back(std::make_unique<QRegister>(n));
        qalloc_count.push_back(1);
    }
    return id;
}

bool MemoryManager::release_qregister(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        return false;
    qregs[id].reset();
    if (id < static_cast<int>(qalloc_count.size()))
        ++qalloc_count[id];
    free_qids.push_back(id);
    return true;
}

int MemoryManager::create_cregister(size_t n) {
    std::lock_guard<std::mutex> lock(mtx);
    int id;
    if (!free_cids.empty()) {
        id = free_cids.back();
        free_cids.pop_back();
        if (id >= static_cast<int>(cregs.size()))
            cregs.resize(id + 1);
        cregs[id] = std::make_unique<CRegister>(n);
        if (id >= static_cast<int>(calloc_count.size()))
            calloc_count.resize(id + 1, 0);
        ++calloc_count[id];
    } else {
        id = static_cast<int>(cregs.size());
        cregs.push_back(std::make_unique<CRegister>(n));
        calloc_count.push_back(1);
    }
    return id;
}

bool MemoryManager::release_cregister(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(cregs.size()) || !cregs[id])
        return false;
    cregs[id].reset();
    if (id < static_cast<int>(calloc_count.size()))
        ++calloc_count[id];
    free_cids.push_back(id);
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

size_t MemoryManager::qreg_allocs(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(qalloc_count.size())) return 0;
    return qalloc_count[id];
}

size_t MemoryManager::creg_allocs(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(calloc_count.size())) return 0;
    return calloc_count[id];
}

size_t MemoryManager::memory_usage() {
    std::lock_guard<std::mutex> lock(mtx);
    size_t bytes = 0;
    for (const auto& q : qregs) {
        if (q) bytes += q->wf.state.size() * sizeof(std::complex<double>);
    }
    for (const auto& c : cregs) {
        if (c) bytes += c->bits.size() * sizeof(int);
    }
    return bytes;
}

std::vector<std::complex<double>> MemoryManager::export_state(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        return {};
    qregs[id]->wf.decompress();
    return qregs[id]->wf.state;
}

bool MemoryManager::import_state(int id, const std::vector<std::complex<double>>& st) {
    std::lock_guard<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        return false;
    qregs[id]->wf.decompress();
    if (st.size() != qregs[id]->wf.state.size()) return false;
    qregs[id]->wf.state = st;
    return true;
}

bool MemoryManager::save_state_to_file(int id, const std::string& path) {
    std::vector<std::complex<double>> st;
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
            return false;
        st = qregs[id]->wf.state;
    }
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) return false;
    size_t n = st.size();
    ofs.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(st.data()), n * sizeof(std::complex<double>));
    return true;
}

bool MemoryManager::load_state_from_file(int id, const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return false;
    size_t n;
    ifs.read(reinterpret_cast<char*>(&n), sizeof(size_t));
    std::vector<std::complex<double>> st(n);
    ifs.read(reinterpret_cast<char*>(st.data()), n * sizeof(std::complex<double>));
    return import_state(id, st);
}

bool MemoryManager::checkpoint_if_needed(int id, std::size_t op_threshold,
                                         double time_threshold_sec,
                                         const std::string& file) {
    std::unique_lock<std::mutex> lock(mtx);
    if (id < 0 || id >= static_cast<int>(qregs.size()) || !qregs[id])
        return false;
    QRegister& qr = *qregs[id];
    bool should = false;
    if (op_threshold > 0 && qr.op_count >= op_threshold)
        should = true;
    if (time_threshold_sec > 0 &&
        qr.elapsed_seconds() >= time_threshold_sec)
        should = true;
    if (!should) return false;
    std::vector<std::complex<double>> st = qr.wf.state;
    qr.reset_metrics();
    lock.unlock();
    std::ofstream ofs(file, std::ios::binary);
    if (!ofs) return false;
    size_t n = st.size();
    ofs.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
    ofs.write(reinterpret_cast<const char*>(st.data()), n * sizeof(std::complex<double>));
    return true;
}

MemoryManager memory;
// Register import/export implemented above
} // namespace qpp

