#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <complex>
#include <chrono>
#include <string>
#include "wavefunction.h"

namespace qpp {
struct QRegister {
    Wavefunction wf;
    std::size_t op_count{0};
    std::chrono::steady_clock::time_point start_time;

    explicit QRegister(size_t n)
        : wf(n), start_time(std::chrono::steady_clock::now()) {}

    void h(std::size_t q) { ++op_count; wf.apply_h(q); }
    void x(std::size_t q) { ++op_count; wf.apply_x(q); }
    void y(std::size_t q) { ++op_count; wf.apply_y(q); }
    void z(std::size_t q) { ++op_count; wf.apply_z(q); }
    void cnot(std::size_t c, std::size_t t) { ++op_count; wf.apply_cnot(c, t); }
    void cz(std::size_t c, std::size_t t) { ++op_count; wf.apply_cz(c, t); }
    void ccnot(std::size_t c1, std::size_t c2, std::size_t t) { ++op_count; wf.apply_ccnot(c1, c2, t); }
    void s(std::size_t q) { ++op_count; wf.apply_s(q); }
    void t(std::size_t q) { ++op_count; wf.apply_t(q); }
    void swap(std::size_t a, std::size_t b) { ++op_count; wf.apply_swap(a, b); }
    int measure(std::size_t q) { ++op_count; return wf.measure(q); }
    std::size_t measure(const std::vector<std::size_t>& qs) { op_count += qs.size(); return wf.measure(qs); }
    void reset() { wf.reset(); reset_metrics(); }
    std::complex<double> amp(std::size_t idx) const { return wf.amplitude(idx); }
    void resize(std::size_t n) { wf = Wavefunction(n); reset_metrics(); }

    std::size_t ops() const { return op_count; }
    double elapsed_seconds() const {
        return std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count();
    }
    void reset_metrics() {
        op_count = 0;
        start_time = std::chrono::steady_clock::now();
    }
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
    // statistics helpers
    size_t qreg_allocs(int id);
    size_t creg_allocs(int id);

    // state import/export
    std::vector<std::complex<double>> export_state(int id);
    bool import_state(int id, const std::vector<std::complex<double>>& st);
    bool save_state_to_file(int id, const std::string& path);
    bool load_state_from_file(int id, const std::string& path);
    bool checkpoint_if_needed(int id, std::size_t op_threshold,
                              double time_threshold_sec,
                              const std::string& file);
private:
    std::vector<std::unique_ptr<QRegister>> qregs;
    std::vector<std::unique_ptr<CRegister>> cregs;
    std::vector<size_t> qalloc_count;
    std::vector<size_t> calloc_count;
    std::vector<int> free_qids;
    std::vector<int> free_cids;
    std::mutex mtx;
};

// TODO(good-first-issue): implement register reuse and bulk operations

extern MemoryManager memory;
}

