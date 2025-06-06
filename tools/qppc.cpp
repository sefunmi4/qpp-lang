#include <iostream>
#include <fstream>
#include <regex>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: qppc <source.qpp>\n";
        return 1;
    }
    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Failed to open " << argv[1] << "\n";
        return 1;
    }
    std::string line;
    std::regex task_regex(R"(task<\s*(CPU|QPU|AUTO)\s*>\s*(\w+)\s*\()");
    while (std::getline(input, line)) {
        std::smatch match;
        if (std::regex_search(line, match, task_regex)) {
            std::cout << "Found task '" << match[2] << "' targeting " << match[1] << "\n";
        }
    }
    std::cout << "Parsing complete." << std::endl;
    return 0;
}
