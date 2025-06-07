#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <string>

namespace qpp {
enum class Target { CPU, QPU, AUTO };

struct Task {
    std::string name;
    Target target;
    std::function<void()> handler;
};

class Scheduler {
public:
    void add_task(const Task& t);
    void run();
private:
    std::queue<Task> tasks;
    std::mutex mtx;
};

// TODO(good-first-issue): extend Scheduler with task priorities and optional
// asynchronous execution support

extern Scheduler scheduler;
} // namespace qpp

