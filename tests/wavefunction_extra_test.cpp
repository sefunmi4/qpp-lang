#include "../runtime/wavefunction.h"
#include <cassert>
#include <iostream>
#include <complex>
#include <cmath>
#include "../runtime/random.h"

int main() {
    using namespace qpp;
    seed_rng(42);
    Wavefunction<float> wf(3);
    wf.apply_x(0);
    wf.apply_y(1);
    wf.apply_s(0);
    wf.apply_t(1);
    wf.apply_cz(0,1);
    wf.apply_swap(0,1);
    wf.apply_ccnot(0,1,2);
    wf.reset();
    auto amp_after = wf.amplitude(0);
    assert(std::abs(amp_after - std::complex<float>(1.0,0.0)) < 1e-6);
    int m0 = wf.measure(0);
    int m1 = wf.measure(1);
    assert(m0 == 0 || m0 == 1);
    assert(m1 == 0 || m1 == 1);
    std::cout << "Extra gate tests passed." << std::endl;
    return 0;
}
