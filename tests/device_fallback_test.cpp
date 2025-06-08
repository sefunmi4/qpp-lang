#include "../runtime/wavefunction.h"
#include "../runtime/device.h"
#include <cassert>
#include <iostream>

int main() {
    using namespace qpp;
    set_device(DeviceType::GPU);
    Wavefunction wf(1);
    wf.apply_x(0);
    // regardless of GPU availability, amplitude should be |1>
    assert(std::abs(std::norm(wf.state[1]) - 1.0) < 1e-6);
    std::cout << "Device fallback test passed." << std::endl;
    return 0;
}
