#include "../runtime/scheduler.h"
#include "../runtime/memory.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

// Simple interpreter for the toy IR emitted by qppc.

using namespace qpp;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: qpp-run <compiled.ir>\n";
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Failed to open " << argv[1] << "\n";
        return 1;
    }
    std::string line;
    std::string current_name;
    Target current_target = Target::AUTO;
    std::vector<std::vector<std::string>> ops;

    auto add_current_task = [&]() {
        if (current_name.empty()) return;
        auto instrs = ops;
        auto name = current_name;
        auto target = current_target;
        scheduler.add_task({name, target, [instrs]() {
            std::unordered_map<std::string,int> qmap;
            std::unordered_map<std::string,int> cmap;
            for (const auto& ins : instrs) {
                if (ins.empty()) continue;
                if (ins[0] == "QALLOC" && ins.size() == 3) {
                    int id = memory.create_qregister(std::stoi(ins[2]));
                    qmap[ins[1]] = id;
                } else if (ins[0] == "CALLOC" && ins.size() == 3) {
                    int id = memory.create_cregister(std::stoi(ins[2]));
                    cmap[ins[1]] = id;
                } else if (ins[0] == "H" || ins[0] == "X" || ins[0] == "Y" || ins[0] == "Z" || ins[0] == "S" || ins[0] == "T") {
                    int id = qmap.at(ins[1]);
                    std::size_t q = std::stoul(ins[2]);
                    if (ins[0] == "H") memory.qreg(id).h(q);
                    else if (ins[0] == "X") memory.qreg(id).x(q);
                    else if (ins[0] == "Y") memory.qreg(id).y(q);
                    else if (ins[0] == "Z") memory.qreg(id).z(q);
                    else if (ins[0] == "S") memory.qreg(id).s(q);
                    else if (ins[0] == "T") memory.qreg(id).t(q);
                } else if (ins[0] == "SWAP" && ins.size() == 5) {
                    int id1 = qmap.at(ins[1]);
                    int id2 = qmap.at(ins[3]);
                    memory.qreg(id1).swap(std::stoul(ins[2]), std::stoul(ins[4]));
                } else if (ins[0] == "CNOT" && ins.size() == 5) {
                    int c = qmap.at(ins[1]);
                    int t = qmap.at(ins[3]);
                    memory.qreg(c).cnot(std::stoul(ins[2]), std::stoul(ins[4]));
                } else if (ins[0] == "MEASURE") {
                    int qid = qmap.at(ins[1]);
                    std::size_t qidx = std::stoul(ins[2]);
                    int result = memory.qreg(qid).measure(qidx);
                    if (ins.size() == 6 && ins[3] == "->") {
                        int cid = cmap.at(ins[4]);
                        std::size_t cidx = std::stoul(ins[5]);
                        if (cidx < memory.creg(cid).bits.size())
                            memory.creg(cid).bits[cidx] = result;
                    }
                }
            }
            for (auto& [name, id] : qmap) memory.release_qregister(id);
            for (auto& [name, id] : cmap) memory.release_cregister(id);
        }});
        ops.clear();
        current_name.clear();
    };

    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::string tok;
        iss >> tok;
        if (tok == "TASK") {
            add_current_task();
            iss >> current_name >> tok; // tok is target
            if (tok == "CPU") current_target = Target::CPU;
            else if (tok == "QPU") current_target = Target::QPU;
            else current_target = Target::AUTO;
        } else if (tok == "ENDTASK") {
            add_current_task();
        } else if (!tok.empty()) {
            std::vector<std::string> parts;
            parts.push_back(tok);
            std::string s;
            while (iss >> s) parts.push_back(s);
            ops.push_back(parts);
        }
    }
    add_current_task();
    scheduler.run();
    // TODO(good-first-issue): surface execution stats and measurement results
    return 0;
}
