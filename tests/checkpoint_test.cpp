#include "../runtime/memory.h"
#include <cassert>
#include <cstdio>
#include <iostream>

using namespace qpp;

int main() {
    int id = memory.create_qregister(1);
    memory.qreg(id).h(0);
    memory.qreg(id).x(0);
    bool cp = memory.checkpoint_if_needed(id, 2, 0.0, "cp.bin");
    assert(cp);
    assert(memory.qreg(id).ops() == 0);
    memory.qreg(id).z(0); // mutate after checkpoint
    bool loaded = memory.load_state_from_file(id, "cp.bin");
    assert(loaded);
    std::remove("cp.bin");
    int m = memory.qreg(id).measure(0);
    assert(m == 0 || m == 1);
    memory.release_qregister(id);
    std::cout << "Checkpoint resume test passed." << std::endl;
    return 0;
}
