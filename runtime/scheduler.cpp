#include "scheduler.h"
#include "memory.h"
#include "memory_tracker.h"
#include "hardware_api.h"
#include "logger.h"
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
            if (tasks.empty() || !running) break;
            if (paused) { continue; }
            t = tasks.top();
            tasks.pop();
        }
        std::string msg = "Running task '" + t.name + "' on ";
        switch (t.target) {
        case Target::CPU:
            msg += "CPU";
            break;
        case Target::QPU:
            msg += "QPU";
            break;
        case Target::AUTO:
            msg += "AUTO";
            break;
        case Target::MIXED:
            msg += "MIXED";
            break;
        }
        if (t.hint == ExecHint::CLIFFORD)
            msg += " [CLIFFORD]";
        else if (t.hint == ExecHint::DENSE)
            msg += " [DENSE]";
        LOG_INFO(msg);
        if (t.handler)
            t.handler();
        if (t.target == Target::QPU && qpu_backend())
            qpu_backend()->execute_qir("; scheduler dispatch\n");
        auto mem = memory.memory_usage();
        LOG_DEBUG("Memory in use: ", mem, " bytes");
        memory_tracker.record(mem);
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
                if (paused) {
                    continue;
                }
                t = tasks.top();
                tasks.pop();
            }
            std::string msg = "Running task '" + t.name + "' on ";
            switch (t.target) {
            case Target::CPU:
                msg += "CPU";
                break;
            case Target::QPU:
                msg += "QPU";
                break;
            case Target::AUTO:
                msg += "AUTO";
                break;
            case Target::MIXED:
                msg += "MIXED";
                break;
            }
            if (t.hint == ExecHint::CLIFFORD)
                msg += " [CLIFFORD]";
            else if (t.hint == ExecHint::DENSE)
                msg += " [DENSE]";
            LOG_INFO(msg);
            if (t.handler)
                t.handler();
            if (t.target == Target::QPU && qpu_backend())
                qpu_backend()->execute_qir("; scheduler dispatch\n");
            auto mem = memory.memory_usage();
            LOG_DEBUG("Memory in use: ", mem, " bytes");
            memory_tracker.record(mem);
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

void Scheduler::stop() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
        cv.notify_all();
    }
    if (worker.joinable())
        worker.join();
}

void Scheduler::pause() {
    std::lock_guard<std::mutex> lock(mtx);
    paused = true;
}

void Scheduler::resume() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        paused = false;
    }
    cv.notify_all();
}

Scheduler scheduler;
// pause/stop controls implemented
} // namespace qpp

