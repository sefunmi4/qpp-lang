#include "scheduler.h"
#include <iostream>

namespace qpp {
void Scheduler::add_task(const Task& t) {
    tasks.push(t);
}

void Scheduler::run() {
    while (!tasks.empty()) {
        Task t = tasks.front();
        tasks.pop();
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
} // namespace qpp

