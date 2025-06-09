#ifndef QPP_WAVEFUNCTION_H
#define QPP_WAVEFUNCTION_H

#include "disk_pager.h"
#include "runtime_config.h"
#include <complex>
#include <vector>
#include <string>
#include <cstddef>
#include <memory>
#include <vector>
#include <unordered_map>

namespace qpp {
template<typename Real = double>
class Wavefunction {
public:
    explicit Wavefunction(std::size_t qubits = 1);

    void apply_h(std::size_t qubit);
    void apply_x(std::size_t qubit);
    void apply_y(std::size_t qubit);
    void apply_z(std::size_t qubit);
    void apply_rx(std::size_t qubit, Real theta);
    void apply_ry(std::size_t qubit, Real theta);
    void apply_rz(std::size_t qubit, Real theta);
    void apply_cnot(std::size_t control, std::size_t target);
    void apply_cz(std::size_t control, std::size_t target);
    void apply_ccnot(std::size_t c1, std::size_t c2, std::size_t target);
    void apply_s(std::size_t qubit);
    void apply_t(std::size_t qubit);
    void apply_swap(std::size_t q1, std::size_t q2);

    // Apply a sequence of single qubit gates by fusing them into one matrix.
    void apply_fused(const std::vector<std::string>& gates,
                     std::size_t qubit);

    void reset();
    std::complex<Real> amplitude(std::size_t index) const;

    int measure(std::size_t qubit);
    std::size_t measure(const std::vector<std::size_t>& qubits);

    void compress();
    void decompress();
    std::size_t nnz() const;
    bool using_sparse() const;
    bool uses_disk() const { return disk_backed; }

    // Detect low entanglement for a single qubit and, when nearly separable,
    // approximate the state via a rank-1 Schmidt decomposition. Returns true
    // if the decomposition was applied.
    bool schmidt_low_rank(std::size_t qubit, double threshold = 1e-6);

  std::vector<std::complex<Real>> state;
  std::unordered_map<std::size_t, std::complex<Real>> sparse_state;
  bool is_sparse{false};
  std::unique_ptr<DiskPager> pager;
  bool disk_backed{false};
  std::size_t num_qubits;
};

// Analyze amplitude magnitudes using a naive discrete Fourier scan and return
// the dominant repeating period. When no strong periodic component is found the
// function returns 0. The threshold parameter specifies the minimum normalised
// Fourier magnitude required for detection.
template<typename Real = double>
std::size_t detect_periodicity_ripple(const Wavefunction<Real>& wf,
                                      double threshold = 0.05);

using WavefunctionF = Wavefunction<float>;

// TODO(good-first-issue): extend with parameterized rotations and register
// import/export helpers
} // namespace qpp

#endif // QPP_WAVEFUNCTION_H
