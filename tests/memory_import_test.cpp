#include "../runtime/memory.h"
#include <cassert>
#include <iostream>
#include "../runtime/random.h"

using namespace qpp;

int main() {
    seed_rng(42);
    int id = memory.create_qregister(1);
    memory.qreg(id).x(0);
    auto st = memory.export_state(id);
    memory.qreg(id).h(0);
    bool ok = memory.import_state(id, st);
    assert(ok);
    int m = memory.qreg(id).measure(0);
    assert(m == 1); // should collapse to |1>
    memory.release_qregister(id);
    std::cout << "Memory import/export test passed." << std::endl;
    return 0;
}

