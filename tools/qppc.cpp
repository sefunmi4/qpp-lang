#include <fstream>
#include <iostream>
#include <regex>
#include <string>

// Very small parser generating a trivial IR used by qpp-run.
// TODO(good-first-issue): replace with a proper frontend when the language
// design stabilises.

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: qppc <source.qpp> <output.ir>\n";
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Failed to open " << argv[1] << "\n";
        return 1;
    }
    std::ofstream out(argv[2]);
    if (!out.is_open()) {
        std::cerr << "Failed to create " << argv[2] << "\n";
        return 1;
    }
    std::regex task_regex(R"(task<\s*(CPU|QPU|AUTO)\s*>\s*(\w+)\s*\()") ;
    std::regex qalloc_regex(R"(qalloc\s+\w+\s+(\w+)\[(\d+)\];)");
    std::regex creg_regex(R"(cregister\s+\w+\s+(\w+)\[(\d+)\];)");
    std::regex gate_regex(R"((H|X|Y|Z|S|T)\((\w+)\[(\d+)\]\);)");
    std::regex swap_regex(R"(SWAP\((\w+)\[(\d+)\],\s*(\w+)\[(\d+)\]\);)");
    std::regex cnot_regex(R"(CX\((\w+)\[(\d+)\],\s*(\w+)\[(\d+)\]\);)");
    std::regex meas_assign_regex(R"((\w+)\[(\d+)\]\s*=\s*measure\((\w+)\[(\d+)\]\);)");
    std::regex measure_regex(R"(measure\((\w+)\[(\d+)\]\);)");

    std::string line;
    bool in_task = false;
    while (std::getline(input, line)) {
        std::smatch m;
        if (std::regex_search(line, m, task_regex)) {
            out << "TASK " << m[2] << " " << m[1] << "\n";
            in_task = true;
            continue;
        }
        if (in_task && line.find('}') != std::string::npos) {
            out << "ENDTASK\n";
            in_task = false;
            continue;
        }
        if (!in_task) continue;
        if (std::regex_search(line, m, qalloc_regex)) {
            out << "QALLOC " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, creg_regex)) {
            out << "CALLOC " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, gate_regex)) {
            out << m[1] << " " << m[2] << " " << m[3] << "\n";
        } else if (std::regex_search(line, m, swap_regex)) {
            out << "SWAP " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << "\n";
        } else if (std::regex_search(line, m, cnot_regex)) {
            out << "CNOT " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << "\n";
        } else if (std::regex_search(line, m, meas_assign_regex)) {
            out << "MEASURE " << m[3] << " " << m[4] << " -> " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, measure_regex)) {
            out << "MEASURE " << m[1] << " " << m[2] << "\n";
        }
    }
    std::cout << "Compilation complete." << std::endl;
    return 0;
}
