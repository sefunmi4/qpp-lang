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
    void apply_cz(std::size_t control, std::size_t target);
    void apply_ccnot(std::size_t c1, std::size_t c2, std::size_t target);
    void apply_s(std::size_t qubit);
    void apply_t(std::size_t qubit);
    void apply_swap(std::size_t q1, std::size_t q2);

    void reset();
    std::complex<double> amplitude(std::size_t index) const;

    int measure(std::size_t qubit);

    std::vector<std::complex<double>> state;
    std::size_t num_qubits;
};

// TODO(good-first-issue): extend with parameterized rotations and register
// import/export helpers
} // namespace qpp


// TODO(good-first-issue): extend with parameterized rotations and register
// import/export helpers
// namespace qpp


// TODO(good-first-issue): add more multi-qubit gates and state vector utilities
// namespace qpp

// TODO(good-first-issue): add more multi-qubit gates and state vector utilities
// namespace qpp


// TODO(good-first-issue): add more multi-qubit gates and state vector utilities
// namespace qpp


// TODO(good-first-issue): add more multi-qubit gates and state vector utilities
// namespace qpp

// TODO: add more multi-qubit gates and state vector utilities

// namespace qpp
#endif // QPP_WAVEFUNCTION_H
