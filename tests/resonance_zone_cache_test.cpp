#include "../runtime/memory.h"
#include <cassert>
#include <iostream>

using namespace qpp;

int main() {
    int id = memory.create_qregister(1);
    memory.qreg(id).x(0);
    bool ok = memory.save_resonance_zone(id, "zone1");
    assert(ok);
    memory.qreg(id).h(0);
    bool ok2 = memory.load_resonance_zone(id, "zone1");
    assert(ok2);
    int m = memory.qreg(id).measure(0);
    assert(m == 1);
    memory.release_qregister(id);
    std::cout << "Resonance cache test passed." << std::endl;
    return 0;
}
