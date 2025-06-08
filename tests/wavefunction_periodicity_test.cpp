#include "../runtime/wavefunction.h"
#include <cassert>
#include <iostream>

int main() {
    using namespace qpp;
    Wavefunction wf(3);
    wf.state.assign(1ULL << 3, {0.0,0.0});
    for (std::size_t i = 0; i < wf.state.size(); i += 2)
        wf.state[i] = 0.5; // normalized amplitude
    std::size_t period = detect_periodicity_ripple(wf, 0.1);
    assert(period == 2);

    wf.reset();
    period = detect_periodicity_ripple(wf, 0.1);
    assert(period == 0);

    std::cout << "Ripple periodicity analysis test passed." << std::endl;
    return 0;
}
