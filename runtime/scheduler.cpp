#include "scheduler.h"
#include <iostream>
#include <mutex>

namespace qpp {
// TODO(good-first-issue): hook Scheduler into a real QPU execution engine

void Scheduler::add_task(const Task& t) {
    std::lock_guard<std::mutex> lock(mtx);
    tasks.push(t);
}

void Scheduler::run() {
    for (;;) {
        Task t;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (tasks.empty()) break;
            t = tasks.front();
            tasks.pop();
        }
        std::cout << "Running task '" << t.name << "' on ";
        switch (t.target) {
        case Target::CPU:
            std::cout << "CPU";
            break;
        case Target::QPU:
            std::cout << "QPU";
            break;
        case Target::AUTO:
            std::cout << "AUTO";
            break;
        }
        std::cout << std::endl;
        if (t.handler)
            t.handler();
    }
}

Scheduler scheduler;
// TODO(good-first-issue): provide scheduler stop/pause controls

} // namespace qpp

