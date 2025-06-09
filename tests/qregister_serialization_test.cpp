#include "../runtime/memory.h"
#include <cassert>
#include <cstdio>
#include <iostream>

using namespace qpp;

int main() {
    QRegister qr(1);
    qr.x(0);
    bool saved = qr.save_to_file("qr.bin");
    assert(saved);

    qr.h(0); // mutate
    bool loaded = qr.load_from_file("qr.bin");
    assert(loaded);
    std::remove("qr.bin");

    int m = qr.measure(0);
    assert(m == 1);

    std::cout << "QRegister serialization test passed." << std::endl;
    return 0;
}
