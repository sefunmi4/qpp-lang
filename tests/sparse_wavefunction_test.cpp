#include "../runtime/wavefunction.h"
#include "../runtime/sparse_wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    using namespace qpp;
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
    std::cout << "Sparse wavefunction test passed." << std::endl;
    return 0;
}
