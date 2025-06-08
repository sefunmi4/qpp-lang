#ifndef QPP_WAVEFUNCTION_H
#define QPP_WAVEFUNCTION_H

#include "disk_pager.h"
#include "runtime_config.h"
#include <complex>
#include <cstddef>
#include <memory>
#include <vector>

namespace qpp {
template<typename Real = double>
class Wavefunction {
public:
  explicit Wavefunction(std::size_t qubits = 1);

  void apply_h(std::size_t qubit);
  void apply_x(std::size_t qubit);
  void apply_y(std::size_t qubit);
  void apply_z(std::size_t qubit);
  void apply_cnot(std::size_t control, std::size_t target);
  void apply_cz(std::size_t control, std::size_t target);
  void apply_ccnot(std::size_t c1, std::size_t c2, std::size_t target);
  void apply_s(std::size_t qubit);
  void apply_t(std::size_t qubit);
  void apply_swap(std::size_t q1, std::size_t q2);

  void reset();
  std::complex<double> amplitude(std::size_t index) const;

  int measure(std::size_t qubit);
  std::size_t measure(const std::vector<std::size_t> &qubits);

  std::size_t size() const;
  std::complex<double> get(std::size_t idx) const;
  void set(std::size_t idx, const std::complex<double> &val);
  bool uses_disk() const { return disk_backed; }

  std::vector<std::complex<double>> state;
  std::unique_ptr<DiskPager> pager;
  bool disk_backed{false};
  std::size_t num_qubits;
};

using WavefunctionF = Wavefunction<float>;

// TODO(good-first-issue): extend with parameterized rotations and register
// import/export helpers
} // namespace qpp

#endif // QPP_WAVEFUNCTION_H
