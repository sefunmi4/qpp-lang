#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

int main() {
    using namespace qpp;
    Wavefunction<float> wf(3);
    wf.apply_h(0);
    wf.apply_cnot(0,1);
    wf.apply_cnot(1,2); // GHZ state (|000> + |111>)/sqrt(2)

    std::size_t res = wf.measure(std::vector<std::size_t>{0,2});
    double norm = 0.0;
    for (const auto& amp : wf.state) norm += std::norm(amp);
    assert(std::abs(norm - 1.0) < 1e-6);

    if (res == 0) {
        // collapsed to |000>
        for (std::size_t i = 1; i < wf.state.size(); ++i)
            assert(std::norm(wf.state[i]) < 1e-6);
        assert(std::abs(wf.state[0] - std::complex<float>(1.0,0.0)) < 1e-6);
    } else {
        assert(res == 3);
        for (std::size_t i = 0; i < wf.state.size(); ++i)
            if (i != 7) assert(std::norm(wf.state[i]) < 1e-6);
        assert(std::abs(wf.state[7] - std::complex<float>(1.0,0.0)) < 1e-6);
    }

    // remaining qubit should match measured value
    int m1 = wf.measure(1);
    if (res == 0) assert(m1 == 0); else assert(m1 == 1);

    std::cout << "Multi-qubit collapse test passed." << std::endl;
    return 0;
}
