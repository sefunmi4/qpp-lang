#include "../runtime/memory.h"
#include <cassert>
#include <thread>
#include <vector>
#include <iostream>

using namespace qpp;

int main() {
    constexpr int loops = 10;
    std::vector<std::thread> threads;
    for (int i = 0; i < loops; ++i) {
        threads.emplace_back([](){
            int id = memory.create_qregister(1);
            memory.qreg(id).h(0);
            memory.release_qregister(id);
        });
    }
    for (auto& t : threads) t.join();
    std::cout << "Memory manager thread test passed." << std::endl;
    return 0;
}
