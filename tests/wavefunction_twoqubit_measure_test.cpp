#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

int main() {
    using namespace qpp;
    Wavefunction wf(2);
    wf.apply_h(0);
    wf.apply_cnot(0,1);

    std::size_t res = wf.measure(std::vector<std::size_t>{0,1});
    double norm = 0.0;
    for (const auto& amp : wf.state) norm += std::norm(amp);
    assert(std::abs(norm - 1.0) < 1e-9);
    if (res == 0) {
        assert(std::abs(wf.state[0] - std::complex<double>(1.0,0.0)) < 1e-9);
        assert(std::norm(wf.state[3]) < 1e-9);
    } else {
        assert(res == 3);
        assert(std::abs(wf.state[3] - std::complex<double>(1.0,0.0)) < 1e-9);
        assert(std::norm(wf.state[0]) < 1e-9);
    }

    wf.reset();
    wf.apply_h(0);
    wf.apply_cnot(0,1);
    int m0 = wf.measure(0);
    int m1 = wf.measure(1);
    norm = 0.0;
    for (const auto& amp : wf.state) norm += std::norm(amp);
    assert(std::abs(norm - 1.0) < 1e-9);
    if (m0 == 0 && m1 == 0) {
        assert(std::abs(wf.state[0] - std::complex<double>(1.0,0.0)) < 1e-9);
    } else {
        assert(m0 == 1 && m1 == 1);
        assert(std::abs(wf.state[3] - std::complex<double>(1.0,0.0)) < 1e-9);
    }

    std::cout << "Two-qubit measurement tests passed." << std::endl;
    return 0;
}
