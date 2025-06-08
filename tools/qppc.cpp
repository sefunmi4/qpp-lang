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
    std::regex call_regex(R"(\w+\(.*\);)");
    std::regex if_var_gate_single(R"(if\s*\(\s*(\w+)\s*\)\s*\{\s*(H|X|Y|Z|S|T)\((\w+)\[(\d+)\]\);\s*\})");
    std::regex if_creg_gate_single(R"(if\s*\(\s*(\w+)\[(\d+)\]\s*\)\s*\{\s*(H|X|Y|Z|S|T)\((\w+)\[(\d+)\]\);\s*\})");
    std::regex else_regex(R"(\}\s*else\s*\{)");

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
            out << "TASK " << m[2] << " " << m[1] << "\n";
            std::size_t start = line.find('(', m.position(0));
            std::size_t end = line.find(')', start);
            if (start != std::string::npos && end != std::string::npos) {
                std::string params = line.substr(start + 1, end - start - 1);
                std::smatch pm;
                auto begin = std::sregex_iterator(params.begin(), params.end(), param_qreg);
                auto endit = std::sregex_iterator();
                for (auto it = begin; it != endit; ++it) {
                    out << "QALLOC " << (*it)[1] << " " << (*it)[2] << "\n";
                }
                begin = std::sregex_iterator(params.begin(), params.end(), param_creg);
                for (auto it = begin; it != endit; ++it) {
                    out << "CALLOC " << (*it)[1] << " " << (*it)[2] << "\n";
                }
            }
            in_task = true;
            continue;
        }
        auto trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));
        trimmed.erase(trimmed.find_last_not_of(" \t") + 1);
        if (in_task && trimmed == "}" && !cond_active) {
            out << "ENDTASK\n";
            in_task = false;
            continue;
        }
        if (!in_task) continue;

        if (cond_active) {
            if (std::regex_search(line, m, gate_regex)) {
                if (cond_type == "var") {
                    out << (cond_else ? "IFNVAR " : "IFVAR ") << cond_name << " "
                        << m[1] << " " << m[2] << " " << m[3] << "\n";
                } else if (cond_type == "creg") {
                    out << (cond_else ? "IFNC " : "IFC ") << cond_name << " "
                        << cond_index << " " << m[1] << " " << m[2] << " " << m[3]
                        << "\n";
                }
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
            out << "IFVAR " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << "\n";
            continue;
        }
        if (std::regex_search(line, m, if_creg_gate_single)) {
        out << "IFC " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << " " << m[5] << "\n";
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
            out << "QALLOC " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, creg_regex)) {
            out << "CALLOC " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, meas_var_regex)) {
            out << "VAR " << m[1] << "\n";
            out << "MEASURE " << m[2] << " " << m[3] << " -> VAR " << m[1] << "\n";
        } else if (std::regex_search(line, m, gate_regex)) {
            out << m[1] << " " << m[2] << " " << m[3] << "\n";
        } else if (std::regex_search(line, m, swap_regex)) {
            out << "SWAP " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << "\n";
        } else if (std::regex_search(line, m, cnot_regex)) {
            out << "CNOT " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << "\n";
        } else if (std::regex_search(line, m, cz_regex)) {
            out << "CZ " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << "\n";
        } else if (std::regex_search(line, m, ccx_regex)) {
            out << "CCX " << m[1] << " " << m[2] << " " << m[3] << " " << m[4] << " " << m[5] << " " << m[6] << "\n";
        } else if (std::regex_search(line, m, xor_assign_regex)) {
            out << "CNOT " << m[3] << " " << m[4] << " " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, meas_assign_regex)) {
            out << "MEASURE " << m[3] << " " << m[4] << " -> " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, measure_regex)) {
            out << "MEASURE " << m[1] << " " << m[2] << "\n";
        } else if (std::regex_search(line, m, call_regex)) {
            // ignore simple task calls
        } else if (trimmed.size() > 0) {
            std::cerr << "Unrecognized syntax on line " << line_no << ": " << trimmed << "\n";
        }
    }
    std::cout << "Compilation complete." << std::endl;
    return 0;
}
