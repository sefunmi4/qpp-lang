#include "../runtime/wavefunction.h"
#include "../runtime/quidd.h"
#include <cassert>
#include <iostream>
#include <cmath>

int main() {
    using namespace qpp;
    // create GHZ state on 8 qubits
    Wavefunction wf(8);
    wf.apply_h(0);
    for (std::size_t q=1; q<8; ++q) wf.apply_cnot(0, q);

    QuIDD dd(wf.state);
    auto vec = dd.to_vector();
    assert(vec.size() == wf.state.size());
    for (std::size_t i=0;i<vec.size();++i)
        assert(std::abs(vec[i]-wf.state[i]) < 1e-12);

    std::size_t wf_bytes = wf.state.size() * sizeof(std::complex<double>);
    std::size_t dd_bytes = dd.memory_bytes();
    assert(dd_bytes < wf_bytes);

    std::cout << "QuIDD conversion test passed." << std::endl;
    return 0;
}
