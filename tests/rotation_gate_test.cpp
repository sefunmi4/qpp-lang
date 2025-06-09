#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace qpp;

int main() {
    Wavefunction<float> wf(1);
    wf.apply_rx(0, M_PI);
    assert(std::abs(std::norm(wf.state[1]) - 1.0f) < 1e-6);

    wf.reset();
    wf.apply_ry(0, M_PI);
    assert(std::abs(std::norm(wf.state[1]) - 1.0f) < 1e-6);

    wf.reset();
    wf.apply_rz(0, 2 * M_PI);
    assert(std::abs(wf.state[0] + std::complex<float>(1.0f, 0.0f)) < 1e-6);

    std::cout << "Rotation gate tests passed." << std::endl;
    return 0;
}
