#include <fstream>
#include <iostream>
#include <regex>
#include <string>

// TODO: replace this stub with a real parser and IR generator

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
    std::regex task_regex(R"(task<\s*(CPU|QPU|AUTO)\s*>\s*(\w+)\s*\()");
    std::string line;
    while (std::getline(input, line)) {
        std::smatch match;
        if (std::regex_search(line, match, task_regex)) {
            out << "TASK " << match[1] << " " << match[2] << "\n";
        }
    }
    std::cout << "Compilation complete." << std::endl;
    // TODO: output intermediate representation once IR design is finalized
    return 0;
}
