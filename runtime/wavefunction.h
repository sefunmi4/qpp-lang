#ifndef QPP_WAVEFUNCTION_H
#define QPP_WAVEFUNCTION_H

#include <complex>
#include <vector>
#include <cstddef>

namespace qpp {
class Wavefunction {
public:
    explicit Wavefunction(std::size_t qubits = 1);

    void apply_h(std::size_t qubit);
    void apply_x(std::size_t qubit);
    void apply_y(std::size_t qubit);
    void apply_z(std::size_t qubit);
    void apply_cnot(std::size_t control, std::size_t target);

    int measure(std::size_t qubit);

    std::vector<std::complex<double>> state;
    std::size_t num_qubits;
};

// TODO: add more gates (S, T, SWAP) and state vector utilities
} // namespace qpp

#endif // QPP_WAVEFUNCTION_H
