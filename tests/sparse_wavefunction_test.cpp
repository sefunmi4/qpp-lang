#include "../runtime/wavefunction.h"
#include "../runtime/sparse_wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include "../runtime/random.h"

int main() {
    using namespace qpp;
    seed_rng(42);

    // Test H and CNOT gates
    {
        Wavefunction dense(2);
        SparseWavefunction sparse(2);

        dense.apply_h(0);
        sparse.apply_h(0);
        dense.apply_cnot(0,1);
        sparse.apply_cnot(0,1);

        for (std::size_t i = 0; i < (1ULL << 2); ++i) {
            auto d = dense.state[i];
            auto s = sparse.amplitude(i);
            assert(std::abs(d - s) < 1e-9);
        }

        int m = sparse.measure(0);
        assert(m == 0 || m == 1);
        std::cout << "Sparse wavefunction test passed.\n";
    }

    // Test Y gate
    {
        Wavefunction dense(1);
        SparseWavefunction sparse(1);
        
        dense.apply_y(0);
        sparse.apply_y(0);
        
        assert(std::abs(dense.state[0] - sparse.amplitude(0)) < 1e-9);
        assert(std::abs(dense.state[1] - sparse.amplitude(1)) < 1e-9);
        assert(std::abs(sparse.amplitude(0)) < 1e-9);
        assert(std::abs(sparse.amplitude(1) - std::complex<double>(0, 1)) < 1e-9);
        
        dense.apply_y(0);
        sparse.apply_y(0);
        
        assert(std::abs(dense.state[0] - sparse.amplitude(0)) < 1e-9);
        assert(std::abs(dense.state[1] - sparse.amplitude(1)) < 1e-9);
        assert(std::abs(sparse.amplitude(0) - std::complex<double>(1, 0)) < 1e-9);
        assert(std::abs(sparse.amplitude(1)) < 1e-9);
        
        std::cout << "Y gate test passed.\n";
    }

    // Test Y, S, and T gates
    {
        Wavefunction dense(2);
        SparseWavefunction sparse(2);
        
        dense.apply_y(0);
        sparse.apply_y(0);
        dense.apply_s(1);
        sparse.apply_s(1);
        dense.apply_t(0);
        sparse.apply_t(0);

        dense.apply_h(1);
        sparse.apply_h(1);
        
        dense.apply_y(1);
        sparse.apply_y(1);
        dense.apply_s(0);
        sparse.apply_s(0);
        dense.apply_t(1);
        sparse.apply_t(1);
        
        for (std::size_t i = 0; i < (1ULL << 2); ++i) {
            auto d = dense.state[i];
            auto s = sparse.amplitude(i);
            assert(std::abs(d - s) < 1e-9);
        }
        
        std::cout << "Y, S, T gates test passed.\n";
    }

    // Combined gates test: H, CNOT, Y, S, T
    {
        Wavefunction dense(2);
        SparseWavefunction sparse(2);
        
        dense.apply_h(0);
        sparse.apply_h(0);
        dense.apply_cnot(0, 1);
        sparse.apply_cnot(0, 1);
        
        dense.apply_y(0);
        sparse.apply_y(0);
        dense.apply_s(1);
        sparse.apply_s(1);
        dense.apply_t(0);
        sparse.apply_t(0);
        
        for (std::size_t i = 0; i < (1ULL << 2); ++i) {
            auto d = dense.state[i];
            auto s = sparse.amplitude(i);
            assert(std::abs(d - s) < 1e-9);
        }
        
        std::cout << "Combined gates test passed.\n";
    }

    return 0;
}