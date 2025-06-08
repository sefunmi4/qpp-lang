#include "../runtime/hardware_api.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

int main() {
    using namespace qpp;
    std::vector<std::vector<std::string>> ops = {{"H","q","0"},{"MEASURE","q","0"}};
    std::string qir = emit_qir(ops);
    assert(qir.find("__quantum__qis__H") != std::string::npos);
    CirqBackend cirq; cirq.execute_qir(qir);
    NvidiaBackend nvidia; nvidia.execute_qir(qir);
    QSharpBackend qsharp; qsharp.execute_qir(qir);
    BraketBackend braket; braket.execute_qir(qir);
    PsiBackend psi; psi.execute_qir(qir);
    std::cout << "Hardware QIR generation test passed." << std::endl;
    return 0;
}
