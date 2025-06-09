#include "../runtime/wavefunction.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    using namespace qpp;
    Wavefunction wf(2);
    wf.apply_h(0); // product state
    bool ok = wf.schmidt_low_rank(0, 1e-6);
    assert(ok);
    double norm = 0.0;
    for (auto &amp : wf.state) norm += std::norm(amp);
    assert(std::abs(norm - 1.0) < 1e-9);

    Wavefunction ent(2);
    ent.apply_h(0);
    ent.apply_cnot(0,1); // maximally entangled
    bool ok2 = ent.schmidt_low_rank(0, 1e-6);
    assert(!ok2);
    norm = 0.0;
    for (auto &amp : ent.state) norm += std::norm(amp);
    assert(std::abs(norm - 1.0) < 1e-9);

    std::cout << "Low rank factorization test passed." << std::endl;
    return 0;
}
