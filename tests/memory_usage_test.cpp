#include "../runtime/memory.h"
#include <cassert>
#include <iostream>

using namespace qpp;

int main() {
    size_t base = memory.memory_usage();
    int id = memory.create_qregister(2);
    memory.qreg(id).wave();
    size_t used = memory.memory_usage();
    assert(used > base);
    memory.release_qregister(id);
    assert(memory.memory_usage() <= base);
    std::cout << "Memory usage test passed." << std::endl;
    return 0;
}
