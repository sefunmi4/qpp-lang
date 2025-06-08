#include "gpu_kernels.h"
#ifdef USE_CUDA
#include <cuda_runtime.h>
#include <cuComplex.h>

namespace qpp {

__global__ void single_qubit_kernel(cuDoubleComplex* state, std::size_t step, std::size_t size,
                                    cuDoubleComplex m00, cuDoubleComplex m01,
                                    cuDoubleComplex m10, cuDoubleComplex m11) {
    std::size_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    std::size_t start = idx * step * 2;
    if (start >= size) return;
    for (std::size_t j = 0; j < step; ++j) {
        std::size_t pos0 = start + j;
        std::size_t pos1 = start + j + step;
        cuDoubleComplex a = state[pos0];
        cuDoubleComplex b = state[pos1];
        state[pos0] = cuCadd(cuCmul(m00, a), cuCmul(m01, b));
        state[pos1] = cuCadd(cuCmul(m10, a), cuCmul(m11, b));
    }
}

void gpu_apply_single_qubit_gate(std::vector<std::complex<double>>& st,
                                 std::size_t target,
                                 const std::complex<double> mat[2][2]) {
    std::size_t size = st.size();
    std::size_t step = 1ULL << target;
    cuDoubleComplex* d_state;
    cudaMalloc(&d_state, sizeof(cuDoubleComplex)*size);
    cudaMemcpy(d_state, st.data(), sizeof(cuDoubleComplex)*size, cudaMemcpyHostToDevice);
    cuDoubleComplex m00 = make_cuDoubleComplex(mat[0][0].real(), mat[0][0].imag());
    cuDoubleComplex m01 = make_cuDoubleComplex(mat[0][1].real(), mat[0][1].imag());
    cuDoubleComplex m10 = make_cuDoubleComplex(mat[1][0].real(), mat[1][0].imag());
    cuDoubleComplex m11 = make_cuDoubleComplex(mat[1][1].real(), mat[1][1].imag());
    dim3 block(64);
    dim3 grid((size/(step*2)+block.x-1)/block.x);
    single_qubit_kernel<<<grid, block>>>(d_state, step, size, m00, m01, m10, m11);
    cudaMemcpy(st.data(), d_state, sizeof(cuDoubleComplex)*size, cudaMemcpyDeviceToHost);
    cudaFree(d_state);
}

__global__ void cnot_kernel(cuDoubleComplex* state, std::size_t size, std::size_t cbit, std::size_t tbit) {
    std::size_t i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= size) return;
    if ((i & cbit) && !(i & tbit)) {
        std::size_t j = i | tbit;
        cuDoubleComplex tmp = state[i];
        state[i] = state[j];
        state[j] = tmp;
    }
}

void gpu_apply_cnot(std::vector<std::complex<double>>& st,
                    std::size_t control, std::size_t target) {
    std::size_t size = st.size();
    cuDoubleComplex* d_state;
    cudaMalloc(&d_state, sizeof(cuDoubleComplex)*size);
    cudaMemcpy(d_state, st.data(), sizeof(cuDoubleComplex)*size, cudaMemcpyHostToDevice);
    dim3 block(64);
    dim3 grid((size+block.x-1)/block.x);
    cnot_kernel<<<grid, block>>>(d_state, size, 1ULL<<control, 1ULL<<target);
    cudaMemcpy(st.data(), d_state, sizeof(cuDoubleComplex)*size, cudaMemcpyDeviceToHost);
    cudaFree(d_state);
}

} // namespace qpp
#endif
