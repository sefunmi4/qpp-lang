#pragma once
#include <functional>
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
};

// TODO: extend Scheduler with priorities and asynchronous execution

extern Scheduler scheduler;
} // namespace qpp

