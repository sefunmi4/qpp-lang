#include "../runtime/scheduler.h"
#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

using namespace qpp;

int main() {
    std::atomic<int> count{0};
    Task t1{"inc1", Target::CPU, ExecHint::NONE, 0, [&]() { count++; }};
    Task t2{"inc2", Target::CPU, ExecHint::NONE, 0, [&]() { count++; }};
    scheduler.add_task(t1);
    scheduler.add_task(t2);
    scheduler.run_async();
    scheduler.pause();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    assert(count.load() <= 1);
    scheduler.resume();
    scheduler.wait();
    assert(count.load() == 2);
    std::cout << "Scheduler pause/resume test passed." << std::endl;
    return 0;
}

