#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    using namespace qpp;
    // single qubit tests
    Wavefunction wf(1);
    wf.apply_h(0);
    assert(std::abs(std::norm(wf.state[0]) - 0.5) < 1e-6);
    assert(std::abs(std::norm(wf.state[1]) - 0.5) < 1e-6);

    wf.apply_x(0);
    wf.apply_y(0);
    wf.apply_z(0);

    int m = wf.measure(0);
    assert(m == 0 || m == 1);

    // two qubit entanglement
    Wavefunction ent(2);
    ent.apply_h(0);
    ent.apply_cnot(0,1);
    double p00 = std::norm(ent.state[0]);
    double p11 = std::norm(ent.state[3]);
    assert(std::abs(p00 - 0.5) < 1e-6);
    assert(std::abs(p11 - 0.5) < 1e-6);

    std::cout << "Wavefunction simulator tests passed." << std::endl;
    return 0;
}
