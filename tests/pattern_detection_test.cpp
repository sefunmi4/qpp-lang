#include "../runtime/patterns.h"
#include <cassert>
#include <iostream>

using namespace qpp;

int main() {
    // QFT2 pattern
    std::vector<std::vector<std::string>> ops = {
        {"H","q","1"},
        {"CNOT","q","1","q","0"},
        {"S","q","0"},
        {"H","q","0"},
        {"SWAP","q","0","q","1"}
    };
    optimize_patterns(ops);
    assert(ops.size() == 1 && ops[0][0] == "QFT2");
    // Execute and compare with explicit sequence
    int id1 = memory.create_qregister(2);
    apply_qft2(memory.qreg(id1), 0, 1);
    auto st_opt = memory.export_state(id1);
    memory.release_qregister(id1);

    int id2 = memory.create_qregister(2);
    memory.qreg(id2).h(1);
    memory.qreg(id2).cnot(1,0);
    memory.qreg(id2).s(0);
    memory.qreg(id2).h(0);
    memory.qreg(id2).swap(0,1);
    auto st_exp = memory.export_state(id2);
    memory.release_qregister(id2);

    assert(st_opt == st_exp);

    // Grover2 pattern
    std::vector<std::vector<std::string>> gops = {
        {"H","q","0"},
        {"H","q","1"},
        {"CNOT","q","1","q","0"},
        {"Z","q","0"},
        {"CNOT","q","1","q","0"},
        {"H","q","0"},
        {"H","q","1"}
    };
    optimize_patterns(gops);
    assert(gops.size() == 1 && gops[0][0] == "GROVER2");

    int id3 = memory.create_qregister(2);
    apply_grover2(memory.qreg(id3), 0, 1);
    auto st_gopt = memory.export_state(id3);
    memory.release_qregister(id3);

    int id4 = memory.create_qregister(2);
    memory.qreg(id4).h(0);
    memory.qreg(id4).h(1);
    memory.qreg(id4).cnot(1,0);
    memory.qreg(id4).z(0);
    memory.qreg(id4).cnot(1,0);
    memory.qreg(id4).h(0);
    memory.qreg(id4).h(1);
    auto st_gexp = memory.export_state(id4);
    memory.release_qregister(id4);
    assert(st_gopt == st_gexp);

    std::cout << "Pattern detection tests passed." << std::endl;
    return 0;
}
