#pragma once
#include <complex>
#include <vector>
#include <cstddef>

namespace qpp {
#ifdef USE_CUDA
void gpu_apply_single_qubit_gate(std::vector<std::complex<double>>& st,
                                 std::size_t target,
                                 const std::complex<double> mat[2][2]);
void gpu_apply_cnot(std::vector<std::complex<double>>& st,
                    std::size_t control, std::size_t target);
#else
inline void gpu_apply_single_qubit_gate(std::vector<std::complex<double>>& st,
                                        std::size_t target,
                                        const std::complex<double> mat[2][2]) {}
inline void gpu_apply_cnot(std::vector<std::complex<double>>& st,
                           std::size_t control, std::size_t target) {}
#endif
} // namespace qpp
