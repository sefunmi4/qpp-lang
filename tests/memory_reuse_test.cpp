#include "../runtime/memory.h"
#include <cassert>
#include <iostream>

using namespace qpp;

int main() {
    int id1 = memory.create_qregister(1);
    memory.release_qregister(id1);
    size_t before = memory.qreg_allocs(id1);
    int id2 = memory.create_qregister(1);
    assert(id1 == id2);
    assert(memory.qreg_allocs(id2) == before + 1);
    memory.release_qregister(id2);
    std::cout << "Memory reuse test passed." << std::endl;
    return 0;
}
