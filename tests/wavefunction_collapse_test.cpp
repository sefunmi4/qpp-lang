#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    using namespace qpp;
    Wavefunction<float> wf(2);
    wf.apply_h(0);
    wf.apply_h(1);
    int m = wf.measure(0);
    double norm = 0.0;
    for (const auto& amp : wf.state) norm += std::norm(amp);
    assert(std::abs(norm - 1.0) < 1e-6);
    if (m == 0) {
        assert(std::norm(wf.state[1]) < 1e-6);
        assert(std::norm(wf.state[3]) < 1e-6);
    } else {
        assert(std::norm(wf.state[0]) < 1e-6);
        assert(std::norm(wf.state[2]) < 1e-6);
    }
    std::cout << "Wavefunction collapse test passed." << std::endl;
    return 0;
}
