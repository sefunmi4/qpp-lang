#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: qpp-run <compiled.ir>\n";
        return 1;
    }
    std::cout << "Executing " << argv[1] << " (runtime stub)" << std::endl;
    return 0;
}
