#include "../runtime/random.h"
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

using namespace qpp;

int main() {
    constexpr int N = 8;
    std::vector<uint32_t> before(N);
    std::vector<uint32_t> after(N);

    // seed before launching threads
    seed_rng(123);
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < N; ++i) {
            threads.emplace_back([i,&before]() {
                before[i] = global_rng()();
            });
        }
        for (auto& t : threads) t.join();
    }

    // launch threads first, then seed
    std::atomic<bool> go{false};
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < N; ++i) {
            threads.emplace_back([i,&after,&go]() {
                while(!go.load()) std::this_thread::yield();
                after[i] = global_rng()();
            });
        }
        // seed after threads have started
        seed_rng(123);
        go.store(true);
        for (auto& t : threads) t.join();
    }

    for (int i = 0; i < N; ++i) {
        assert(before[i] == after[i]);
    }

    std::cout << "Random concurrency test passed." << std::endl;
    return 0;
}
