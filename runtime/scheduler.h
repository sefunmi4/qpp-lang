#pragma once
#include <functional>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <string>

namespace qpp {
enum class Target { CPU, QPU, AUTO };

enum class ExecHint { NONE, DENSE, CLIFFORD };

struct Task {
    std::string name;
    Target target;
    ExecHint hint{ExecHint::NONE};
    int priority{0};
    std::function<void()> handler;
};

class Scheduler {
public:
    void add_task(const Task& t);
    void run();
    void run_async();
    void wait();
    void stop();
    void pause();
    void resume();
private:
    struct Compare {
        bool operator()(const Task& a, const Task& b) const {
            return a.priority < b.priority; // higher priority first
        }
    };
    std::priority_queue<Task, std::vector<Task>, Compare> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool running = false;
    bool paused = false;
    std::thread worker;
};

// TODO(good-first-issue): extend Scheduler with task priorities and optional
// asynchronous execution support

extern Scheduler scheduler;
} // namespace qpp

