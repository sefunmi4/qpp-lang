#include "../runtime/wavefunction.h"
#include <cassert>
#include <vector>
#include <string>
#include <complex>
#include <cmath>
#include <iostream>

int main() {
    using namespace qpp;
    Wavefunction w1(1);
    w1.apply_h(0);
    w1.apply_x(0);
    w1.apply_z(0);

    Wavefunction w2(1);
    w2.apply_fused({"H","X","Z"}, 0);

    for (size_t i = 0; i < w1.state.size(); ++i) {
        assert(std::abs(w1.state[i] - w2.state[i]) < 1e-9);
    }
    std::cout << "Fused gate test passed." << std::endl;
    return 0;
}
