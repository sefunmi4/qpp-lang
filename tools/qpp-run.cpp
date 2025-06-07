#include "../runtime/scheduler.h"
#include <fstream>
#include <iostream>
#include <sstream>

// TODO: interpret real IR and invoke CPU/QPU handlers

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
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::string tag, target_str, name;
        iss >> tag >> target_str >> name;
        if (tag != "TASK") continue;
        Target t = Target::AUTO;
        if (target_str == "CPU") t = Target::CPU;
        else if (target_str == "QPU") t = Target::QPU;
        scheduler.add_task({name, t, []{}});
    }
    scheduler.run();
    // TODO: surface execution stats and measurement results
    return 0;
}
