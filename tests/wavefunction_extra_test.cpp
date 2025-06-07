#include "../runtime/wavefunction.h"
#include <cassert>
#include <iostream>

int main() {
    using namespace qpp;
    Wavefunction wf(2);
    wf.apply_x(0);
    wf.apply_y(1);
    wf.apply_s(0);
    wf.apply_t(1);
    wf.apply_swap(0,1);
    int m0 = wf.measure(0);
    int m1 = wf.measure(1);
    assert(m0 == 0 || m0 == 1);
    assert(m1 == 0 || m1 == 1);
    std::cout << "Extra gate tests passed." << std::endl;
    return 0;
}
