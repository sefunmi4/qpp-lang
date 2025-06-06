#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    qpp::Wavefunction wf;
    // apply Hadamard gate to |0>
    wf.apply_h();
    double p0 = std::norm(wf.state[0]);
    double p1 = std::norm(wf.state[1]);
    assert(std::abs(p0 - 0.5) < 1e-6);
    assert(std::abs(p1 - 0.5) < 1e-6);

    // apply X gate; should swap amplitudes
    wf.apply_x();
    assert(std::abs(std::norm(wf.state[0]) - 0.5) < 1e-6);
    assert(std::abs(std::norm(wf.state[1]) - 0.5) < 1e-6);

    std::cout << "Wavefunction simulator tests passed." << std::endl;
    return 0;
}
