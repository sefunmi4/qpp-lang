#include "scheduler.h"
#include <iostream>
#include <mutex>

namespace qpp {
// TODO(good-first-issue): hook Scheduler into a real QPU execution engine
void Scheduler::add_task(const Task& t) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(t);
    }
    cv.notify_one();
}

void Scheduler::run() {
    running = true;
    for (;;) {
        Task t;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (tasks.empty()) break;
            t = tasks.top();
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
    running = false;
}

void Scheduler::run_async() {
    std::lock_guard<std::mutex> lock(mtx);
    if (running) return;
    running = true;
    worker = std::thread([this]() {
        for (;;) {
            Task t;
            {
                std::unique_lock<std::mutex> lk(mtx);
                cv.wait(lk, [this]() { return !tasks.empty() || !running; });
                if (!running && tasks.empty())
                    break;
                t = tasks.top();
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
    });
}

void Scheduler::wait() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
        cv.notify_all();
    }
    if (worker.joinable())
        worker.join();
}

Scheduler scheduler;
// TODO(good-first-issue): provide scheduler stop/pause controls
} // namespace qpp

