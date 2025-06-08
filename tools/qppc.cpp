#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

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
    std::regex param_qreg(R"(qregister(?:\s+\w+)?\s*(\w+)\[(\d+)\])");
    std::regex param_creg(R"(cregister(?:\s+\w+)?\s*(\w+)\[(\d+)\])");
    std::regex qalloc_regex(R"(qalloc\s+\w+\s+(\w+)\[(\d+)\];)");
    std::regex creg_regex(R"(cregister\s+\w+\s+(\w+)\[(\d+)\];)");
    std::regex gate_regex(R"((H|X|Y|Z|S|T)\((\w+)\[(\d+)\]\);)");
    std::regex cz_regex(R"(CZ\((\w+)\[(\d+)\],\s*(\w+)\[(\d+)\]\);)");
    std::regex ccx_regex(R"(CCX\((\w+)\[(\d+)\],\s*(\w+)\[(\d+)\],\s*(\w+)\[(\d+)\]\);)");
    std::regex swap_regex(R"(SWAP\((\w+)\[(\d+)\],\s*(\w+)\[(\d+)\]\);)");
    std::regex cnot_regex(R"(CX\((\w+)\[(\d+)\],\s*(\w+)\[(\d+)\]\);)");
    std::regex meas_assign_regex(R"((\w+)\[(\d+)\]\s*=\s*measure\((\w+)\[(\d+)\]\);)");
    std::regex meas_var_regex(R"(int\s+(\w+)\s*=\s*measure\((\w+)\[(\d+)\]\);)");
    std::regex measure_regex(R"(measure\((\w+)\[(\d+)\]\);)");
    std::regex xor_assign_regex(R"((\w+)\[(\d+)\]\s*\^=\s*(\w+)\[(\d+)\];)");
    std::regex if_var_regex(R"(if\s*\(\s*(\w+)\s*\)\s*\{)");
    std::regex if_creg_regex(R"(if\s*\(\s*(\w+)\[(\d+)\]\s*\)\s*\{)");
    std::regex if_var_gate_single(R"(if\s*\(\s*(\w+)\s*\)\s*\{\s*(H|X|Y|Z|S|T)\((\w+)\[(\d+)\]\);\s*\})");
    std::regex if_creg_gate_single(R"(if\s*\(\s*(\w+)\[(\d+)\]\s*\)\s*\{\s*(H|X|Y|Z|S|T)\((\w+)\[(\d+)\]\);\s*\})");
    std::regex else_regex(R"(\}\s*else\s*\{)");
    std::regex call_regex(R"((\w+)\s*\([^)]*\)\s*;)");

    int total_qubits = 0;
    int total_gates = 0;
    std::vector<std::string> output_lines;

    std::string line;
    int line_no = 0;
    bool in_task = false;
    bool cond_active = false;
    bool cond_else = false;
    bool cond_pending = false; // remember last cond after closing for else
    std::string last_cond_type;
    std::string last_cond_name;
    std::string last_cond_index;
    std::string cond_type; // "var" or "creg"
    std::string cond_name;
    std::string cond_index;

    while (std::getline(input, line)) {
        ++line_no;
        std::smatch m;
        // strip comments
        auto pos = line.find("//");
        if (pos != std::string::npos) line = line.substr(0, pos);
        if (std::regex_search(line, m, task_regex)) {
            output_lines.push_back(std::string("TASK ") + m[2].str() + " " + m[1].str());
            std::size_t start = line.find('(', m.position(0));
            std::size_t end = line.find(')', start);
            if (start != std::string::npos && end != std::string::npos) {
                std::string params = line.substr(start + 1, end - start - 1);
                std::smatch pm;
                auto begin = std::sregex_iterator(params.begin(), params.end(), param_qreg);
                auto endit = std::sregex_iterator();
                for (auto it = begin; it != endit; ++it) {
                    output_lines.push_back(std::string("QALLOC ") + (*it)[1].str() + " " + (*it)[2].str());
                    total_qubits += std::stoi((*it)[2]);
                }
                begin = std::sregex_iterator(params.begin(), params.end(), param_creg);
                for (auto it = begin; it != endit; ++it) {
                    output_lines.push_back(std::string("CALLOC ") + (*it)[1].str() + " " + (*it)[2].str());
                }
            }
            in_task = true;
            continue;
        }
        auto trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
        if (in_task && trimmed == "}" && !cond_active) {
            output_lines.push_back("ENDTASK");
            in_task = false;
            continue;
        }
        if (!in_task) continue;

        if (cond_active) {
            if (std::regex_search(line, m, gate_regex)) {
                if (cond_type == "var") {
                    output_lines.push_back(std::string(cond_else ? "IFNVAR " : "IFVAR ") + cond_name + " " + m[1].str() + " " + m[2].str() + " " + m[3].str());
                } else if (cond_type == "creg") {
                    output_lines.push_back(std::string(cond_else ? "IFNC " : "IFC ") + cond_name + " " + cond_index + " " + m[1].str() + " " + m[2].str() + " " + m[3].str());
                }
                total_gates++;
                continue;
            }
            if (std::regex_search(line, else_regex)) {
                cond_else = true;
                continue;
            }
            if (trimmed == "}") {
                cond_pending = true;
                last_cond_type = cond_type;
                last_cond_name = cond_name;
                last_cond_index = cond_index;
                cond_active = false;
                cond_else = false;
                continue;
            }
            continue;
        }

        if (cond_pending && trimmed == "else {") {
            cond_active = true;
            cond_else = true;
            cond_type = last_cond_type;
            cond_name = last_cond_name;
            cond_index = last_cond_index;
            cond_pending = false;
            continue;
        }

        if (std::regex_search(line, m, if_var_gate_single)) {
            output_lines.push_back(std::string("IFVAR ") + m[1].str() + " " + m[2].str() + " " + m[3].str() + " " + m[4].str());
            total_gates++;
            continue;
        }
        if (std::regex_search(line, m, if_creg_gate_single)) {
            output_lines.push_back(std::string("IFC ") + m[1].str() + " " + m[2].str() + " " + m[3].str() + " " + m[4].str() + " " + m[5].str());
            total_gates++;
            continue;
        }

        if (std::regex_search(line, m, if_var_regex)) {
            cond_active = true;
            cond_else = false;
            cond_type = "var";
            cond_name = m[1];
            continue;
        }
        if (std::regex_search(line, m, if_creg_regex)) {
            cond_active = true;
            cond_else = false;
            cond_type = "creg";
            cond_name = m[1];
            cond_index = m[2];
            continue;
        }
        if (std::regex_search(line, m, qalloc_regex)) {
            output_lines.push_back(std::string("QALLOC ") + m[1].str() + " " + m[2].str());
            total_qubits += std::stoi(m[2]);
        } else if (std::regex_search(line, m, creg_regex)) {
            output_lines.push_back(std::string("CALLOC ") + m[1].str() + " " + m[2].str());
        } else if (std::regex_search(line, m, meas_var_regex)) {
            output_lines.push_back(std::string("VAR ") + m[1].str());
            output_lines.push_back(std::string("MEASURE ") + m[2].str() + " " + m[3].str() + " -> VAR " + m[1].str());
        } else if (std::regex_search(line, m, gate_regex)) {
            output_lines.push_back(std::string(m[1]) + " " + m[2].str() + " " + m[3].str());
            total_gates++;
        } else if (std::regex_search(line, m, swap_regex)) {
            output_lines.push_back(std::string("SWAP ") + m[1].str() + " " + m[2].str() + " " + m[3].str() + " " + m[4].str());
            total_gates++;
        } else if (std::regex_search(line, m, cnot_regex)) {
            output_lines.push_back(std::string("CNOT ") + m[1].str() + " " + m[2].str() + " " + m[3].str() + " " + m[4].str());
            total_gates++;
        } else if (std::regex_search(line, m, cz_regex)) {
            output_lines.push_back(std::string("CZ ") + m[1].str() + " " + m[2].str() + " " + m[3].str() + " " + m[4].str());
            total_gates++;
        } else if (std::regex_search(line, m, ccx_regex)) {
            output_lines.push_back(std::string("CCX ") + m[1].str() + " " + m[2].str() + " " + m[3].str() + " " + m[4].str() + " " + m[5].str() + " " + m[6].str());
            total_gates++;
        } else if (std::regex_search(line, m, xor_assign_regex)) {
            output_lines.push_back(std::string("CNOT ") + m[3].str() + " " + m[4].str() + " " + m[1].str() + " " + m[2].str());
            total_gates++;
        } else if (std::regex_search(line, m, meas_assign_regex)) {
            output_lines.push_back(std::string("MEASURE ") + m[3].str() + " " + m[4].str() + " -> " + m[1].str() + " " + m[2].str());
        } else if (std::regex_search(line, m, measure_regex)) {
            output_lines.push_back(std::string("MEASURE ") + m[1].str() + " " + m[2].str());
        } else if (std::regex_search(line, m, call_regex)) {
            // simple function call ignored in this toy compiler
        } else if (trimmed.size() > 0) {
            std::cerr << "Unrecognized syntax on line " << line_no << ": " << trimmed << "\n";
        }
    }
    out.seekp(0);
    out << "#QUBITS " << total_qubits << "\n";
    out << "#GATES " << total_gates << "\n";
    for (const auto& l : output_lines) out << l << "\n";
    std::cout << "Compilation complete. Estimated qubits: " << total_qubits
              << ", gates: " << total_gates << std::endl;
    return 0;
}
