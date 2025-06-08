#include "../runtime/scheduler.h"
#include "../runtime/memory.h"
#include "../runtime/hardware_api.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>
#include <string>

// Simple interpreter for the toy IR emitted by qppc.

using namespace qpp;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: qpp-run [--use-qiskit|--use-cirq|--use-braket|--use-qsharp|--use-nvidia|--use-psi] <compiled.ir>\n";
        return 1;
    }
    int argi = 1;
    std::string opt = argv[1];
    int header_qubits = -1;
    int header_gates = -1;
    int calc_qubits = 0;
    int calc_gates = 0;
    if (opt.rfind("--use-",0)==0) {
        if (opt == "--use-qiskit") set_qpu_backend(std::make_unique<QiskitBackend>());
        else if (opt == "--use-cirq") set_qpu_backend(std::make_unique<CirqBackend>());
        else if (opt == "--use-braket") set_qpu_backend(std::make_unique<BraketBackend>());
        else if (opt == "--use-qsharp") set_qpu_backend(std::make_unique<QSharpBackend>());
        else if (opt == "--use-nvidia") set_qpu_backend(std::make_unique<NvidiaBackend>());
        else if (opt == "--use-psi") set_qpu_backend(std::make_unique<PsiBackend>());
        else {
            std::cerr << "Unknown backend option " << opt << "\n";
            return 1;
        }
        if (argc < 3) {
            std::cerr << "No IR file provided\n";
            return 1;
        }
        argi = 2;
    }
    std::ifstream input(argv[argi]);
    if (!input.is_open()) {
        std::cerr << "Failed to open " << argv[argi] << "\n";
        return 1;
    }
    std::string line;
    std::string current_name;
    Target current_target = Target::AUTO;
    ExecHint current_hint = ExecHint::NONE;
    std::vector<std::vector<std::string>> ops;

    bool use_stabilizer = false;

    std::vector<std::string> logs;
    const std::vector<std::string> gate_ops = {"H","X","Y","Z","S","T","SWAP","CNOT","CZ","CCX","IFVAR","IFNVAR","IFC","IFNC"};

    auto add_current_task = [&]() {
        if (current_name.empty()) return;
        auto instrs = ops;
        auto name = current_name;
        auto target = current_target;
        auto hint = current_hint;
        scheduler.add_task({name, target, hint, 0, [instrs,&logs,name,target,hint]() {
            if (hint == ExecHint::CLIFFORD)
                std::cout << "[runtime] hint CLIFFORD - using stabilizer path" << std::endl;
            else if (hint == ExecHint::DENSE)
                std::cout << "[runtime] hint DENSE - using dense path" << std::endl;
            if (target == Target::QPU && qpu_backend()) {
                auto qir = emit_qir(instrs);
                qpu_backend()->execute_qir(qir);
            }
            std::unordered_map<std::string,int> qmap;
            std::unordered_map<std::string,int> cmap;
            std::unordered_map<std::string,int> vars;

            auto apply_gate = [&](const std::string& g,
                                  const std::string& qname,
                                  const std::string& qidx) {
                int id = qmap.at(qname);
                std::size_t q = std::stoul(qidx);
                if (g == "H") memory.qreg(id).h(q);
                else if (g == "X") memory.qreg(id).x(q);
                else if (g == "Y") memory.qreg(id).y(q);
                else if (g == "Z") memory.qreg(id).z(q);
                else if (g == "S") memory.qreg(id).s(q);
                else if (g == "T") memory.qreg(id).t(q);
            };

            for (const auto& ins : instrs) {
                if (ins.empty()) continue;
                if (ins[0] == "QALLOC" && ins.size() == 3) {
                    int id = memory.create_qregister(std::stoi(ins[2]));
                    qmap[ins[1]] = id;
                } else if (ins[0] == "CALLOC" && ins.size() == 3) {
                    int id = memory.create_cregister(std::stoi(ins[2]));
                    cmap[ins[1]] = id;
                } else if (ins[0] == "VAR" && ins.size() == 2) {
                    vars[ins[1]] = 0;
                } else if (ins[0] == "H" || ins[0] == "X" || ins[0] == "Y" || ins[0] == "Z" || ins[0] == "S" || ins[0] == "T") {
                    apply_gate(ins[0], ins[1], ins[2]);
                } else if (ins[0] == "SWAP" && ins.size() == 5) {
                    int id1 = qmap.at(ins[1]);
                    int id2 = qmap.at(ins[3]);
                    memory.qreg(id1).swap(std::stoul(ins[2]), std::stoul(ins[4]));
                } else if (ins[0] == "CNOT" && ins.size() == 5) {
                    int c = qmap.at(ins[1]);
                    int t = qmap.at(ins[3]);
                    memory.qreg(c).cnot(std::stoul(ins[2]), std::stoul(ins[4]));
                } else if (ins[0] == "CZ" && ins.size() == 5) {
                    int c = qmap.at(ins[1]);
                    int t = qmap.at(ins[3]);
                    memory.qreg(c).cz(std::stoul(ins[2]), std::stoul(ins[4]));
                } else if (ins[0] == "CCX" && ins.size() == 7) {
                    int c1 = qmap.at(ins[1]);
                    int c2 = qmap.at(ins[3]);
                    int targ = qmap.at(ins[5]); // ensure register exists
                    (void)targ;
                    memory.qreg(c1).ccnot(std::stoul(ins[2]), std::stoul(ins[4]), std::stoul(ins[6]));
                } else if (ins[0] == "MEASURE") {
                    int qid = qmap.at(ins[1]);
                    std::size_t qidx = std::stoul(ins[2]);
                int result = memory.qreg(qid).measure(qidx);
                logs.push_back(name + ": measured " + ins[1] + "[" + ins[2] + "] = " + std::to_string(result));
                if (ins.size() == 6 && ins[3] == "->") {
                    if (ins[4] == "VAR") {
                        vars[ins[5]] = result;
                        } else {
                            int cid = cmap.at(ins[4]);
                            std::size_t cidx = std::stoul(ins[5]);
                            if (cidx < memory.creg(cid).bits.size())
                                memory.creg(cid).bits[cidx] = result;
                        }
                    }
                } else if (ins[0] == "IFVAR" && ins.size() == 5) {
                    bool cond = vars[ins[1]];
                    logs.push_back(name + ": branch IFVAR " + ins[1] + " -> " +
                                   (cond ? "taken" : "skipped"));
                    if (cond)
                        apply_gate(ins[2], ins[3], ins[4]);
                } else if (ins[0] == "IFNVAR" && ins.size() == 5) {
                    bool cond = !vars[ins[1]];
                    logs.push_back(name + ": branch IFNVAR " + ins[1] + " -> " +
                                   (cond ? "taken" : "skipped"));
                    if (cond)
                        apply_gate(ins[2], ins[3], ins[4]);
                } else if (ins[0] == "IFC" && ins.size() == 6) {
                    int cid = cmap.at(ins[1]);
                    std::size_t idx = std::stoul(ins[2]);
                    bool cond = memory.creg(cid).bits[idx];
                    logs.push_back(name + ": branch IFC " + ins[1] + "[" + ins[2]
                                   + "] -> " + (cond ? "taken" : "skipped"));
                    if (cond)
                        apply_gate(ins[3], ins[4], ins[5]);
                } else if (ins[0] == "IFNC" && ins.size() == 6) {
                    int cid = cmap.at(ins[1]);
                    std::size_t idx = std::stoul(ins[2]);
                    bool cond = !memory.creg(cid).bits[idx];
                    logs.push_back(name + ": branch IFNC " + ins[1] + "[" + ins[2]
                                   + "] -> " + (cond ? "taken" : "skipped"));
                    if (cond)
                        apply_gate(ins[3], ins[4], ins[5]);
                }
            }
            for (auto& [name, id] : qmap) memory.release_qregister(id);
            for (auto& [name, id] : cmap) memory.release_cregister(id);
        }});
        ops.clear();
        current_name.clear();
    };

    int line_no = 0;
    while (std::getline(input, line)) {
        ++line_no;
        if (!line.empty() && line[0] == '#')
            continue;
        std::istringstream iss(line);
        std::string tok;
        iss >> tok;
        if (tok == "#QUBITS") {
            iss >> header_qubits;
            continue;
        } else if (tok == "#GATES") {
            iss >> header_gates;
            continue;
        }
        if (tok == "CLIFFORD") {
            int v = 0;
            iss >> v;
            use_stabilizer = (v != 0);
        } else if (tok == "TASK") {
            add_current_task();
            iss >> current_name >> tok; // tok is target
            if (tok == "CPU") current_target = Target::CPU;
            else if (tok == "QPU") current_target = Target::QPU;
            else current_target = Target::AUTO;
            std::string hintTok;
            if (iss >> hintTok) {
                if (hintTok == "DENSE") current_hint = ExecHint::DENSE;
                else if (hintTok == "CLIFFORD") current_hint = ExecHint::CLIFFORD;
                else current_hint = ExecHint::NONE;
            } else {
                current_hint = ExecHint::NONE;
            }
        } else if (tok == "ENDTASK") {
            add_current_task();
        } else if (tok == "ENGINE") {
            std::string eng;
            iss >> eng;
            if (eng == "FULL") current_engine = Engine::FULL;
            else if (eng == "TENSOR") current_engine = Engine::TENSOR;
            else if (eng == "STABILIZER") current_engine = Engine::STABILIZER;
        } else if (!tok.empty()) {
            std::vector<std::string> parts;
            parts.push_back(tok);
            std::string s;
            while (iss >> s) parts.push_back(s);
            ops.push_back(parts);
            if (tok == "QALLOC" && parts.size() >= 3) {
                calc_qubits += std::stoi(parts[2]);
            } else if (std::find(gate_ops.begin(), gate_ops.end(), tok) != gate_ops.end()) {
                if (!(tok == "QALLOC" || tok == "CALLOC" || tok == "MEASURE" || tok == "VAR"))
                    calc_gates++;
            }
        } else if (!line.empty()) {
            std::cerr << "Unknown instruction on line " << line_no << ": " << line << "\n";
        }
    }
    add_current_task();
    int q_est = header_qubits >= 0 ? header_qubits : calc_qubits;
    int g_est = header_gates >= 0 ? header_gates : calc_gates;
    std::cout << "Estimated qubits: " << q_est << ", gates: " << g_est << std::endl;
    scheduler.run();
    for (const auto& l : logs) std::cout << l << std::endl;
    std::cout << "Executed " << logs.size() << " measurements." << std::endl;
    return 0;
}
