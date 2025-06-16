#ifndef QPP_SPARSE_WAVEFUNCTION_H
#define QPP_SPARSE_WAVEFUNCTION_H

#include <complex>
#include <unordered_map>
#include <cstddef>

namespace qpp {
class SparseWavefunction {
public:
    explicit SparseWavefunction(std::size_t qubits = 1);

    void apply_h(std::size_t qubit);
    void apply_x(std::size_t qubit);
    void apply_z(std::size_t qubit);
    void apply_y(std::size_t qubit);
    void apply_s(std::size_t qubit);
    void apply_t(std::size_t qubit);
    void apply_cnot(std::size_t control, std::size_t target);
    int measure(std::size_t qubit);
    void reset();
    std::complex<double> amplitude(std::size_t index) const;

    std::unordered_map<std::size_t, std::complex<double>> state;
    std::size_t num_qubits;
};
} // namespace qpp

#endif // QPP_SPARSE_WAVEFUNCTION_H
