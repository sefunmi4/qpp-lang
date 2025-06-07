#pragma once
#include <vector>
#include "wavefunction.h"

namespace qpp {
struct QRegister {
    Wavefunction wf;
    explicit QRegister(size_t n) : wf(n) {}
};

struct CRegister {
    std::vector<int> bits;
    explicit CRegister(size_t n) : bits(n, 0) {}
};

class MemoryManager {
public:
    int create_qregister(size_t n);
    int create_cregister(size_t n);
    QRegister& qreg(int id);
    CRegister& creg(int id);
private:
    std::vector<QRegister> qregs;
    std::vector<CRegister> cregs;
};

extern MemoryManager memory;
}

