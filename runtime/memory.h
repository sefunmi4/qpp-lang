#pragma once
#include <memory>
#include <mutex>
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
    void cz(std::size_t c, std::size_t t) { wf.apply_cz(c, t); }
    void ccnot(std::size_t c1, std::size_t c2, std::size_t t) { wf.apply_ccnot(c1, c2, t); }
    void s(std::size_t q) { wf.apply_s(q); }
    void t(std::size_t q) { wf.apply_t(q); }
    void swap(std::size_t a, std::size_t b) { wf.apply_swap(a, b); }
    int measure(std::size_t q) { return wf.measure(q); }
    void reset() { wf.reset(); }
    std::complex<double> amp(std::size_t idx) const { return wf.amplitude(idx); }
    void resize(std::size_t n) { wf = Wavefunction(n); }
};

// TODO(good-first-issue): enhance QRegister with save/load helpers and
// optional lazy allocation of the underlying Wavefunction

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
    std::mutex mtx;
};

// TODO(good-first-issue): implement register reuse and bulk operations

extern MemoryManager memory;
}

