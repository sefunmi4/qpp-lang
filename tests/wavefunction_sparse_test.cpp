#include "../runtime/memory.h"
#include <cassert>
#include <iostream>

int main() {
    using namespace qpp;
    QRegister qr(2);
    qr.wave();
    // initial state |00>
    qr.compress();
    assert(qr.using_sparse());
    assert(qr.nnz() == 1);
    qr.decompress();
    qr.x(1); // state |10>
    qr.compress();
    assert(qr.nnz() == 1);
    auto amp = qr.wave().amplitude(2);
    assert(std::abs(amp - std::complex<double>(1.0,0.0)) < 1e-9);
    qr.decompress();
    assert(qr.wave().state[2] == std::complex<double>(1.0,0.0));
    std::cout << "Sparse compression test passed." << std::endl;
    return 0;
}

