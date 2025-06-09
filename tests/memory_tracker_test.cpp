#include "../runtime/memory.h"
#include "../runtime/memory_tracker.h"
#include <cassert>
#include <iostream>

using namespace qpp;

int main() {
    memory_tracker.start();
    int id = memory.create_qregister(1);
    memory.qreg(id).h(0);
    memory_tracker.record(memory.memory_usage());
    memory.release_qregister(id);
    memory_tracker.stop();
    assert(!memory_tracker.samples.empty());
    std::cout << "Memory tracker test passed." << std::endl;
    return 0;
}
