#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    using namespace qpp;
    Wavefunction wf(2);
    wf.apply_h(0);
    wf.apply_h(1);
    int m = wf.measure(0);
    double norm = 0.0;
    for (const auto& amp : wf.state) norm += std::norm(amp);
    assert(std::abs(norm - 1.0) < 1e-9);
    if (m == 0) {
        assert(std::norm(wf.state[1]) < 1e-9);
        assert(std::norm(wf.state[3]) < 1e-9);
    } else {
        assert(std::norm(wf.state[0]) < 1e-9);
        assert(std::norm(wf.state[2]) < 1e-9);
    }
    std::cout << "Wavefunction collapse test passed." << std::endl;
    return 0;
}
