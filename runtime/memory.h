#pragma once
#include <memory>

// TODO: enhance QRegister to support saving/loading state and lazy allocation

// TODO: make MemoryManager thread-safe and support register reuse

#include <vector>
#include "wavefunction.h"

namespace qpp {
struct QRegister {
    Wavefunction wf;
    explicit QRegister(size_t n) : wf(n) {}

    void h(std::size_t q) { wf.apply_h(q); }
    void x(std::size_t q) { wf.apply_x(q); }
    void y(std::size_t q) { wf.apply_y(q); }
    void z(std::size_t q) { wf.apply_z(q); }
    void cnot(std::size_t c, std::size_t t) { wf.apply_cnot(c, t); }
    int measure(std::size_t q) { return wf.measure(q); }
    void resize(std::size_t n) { wf = Wavefunction(n); }
};

struct CRegister {
    std::vector<int> bits;
    explicit CRegister(size_t n) : bits(n, 0) {}
};

class MemoryManager {
public:
    int create_qregister(size_t n);
    bool release_qregister(int id);
    int create_cregister(size_t n);
    bool release_cregister(int id);
    QRegister& qreg(int id);
    CRegister& creg(int id);
private:
    std::vector<std::unique_ptr<QRegister>> qregs;
    std::vector<std::unique_ptr<CRegister>> cregs;
};

extern MemoryManager memory;
}

