#include "../runtime/memory.h"
#include "../runtime/scheduler.h"
#include <iostream>
#include <cassert>

using namespace qpp;

int main() {
    int qid = memory.create_qregister(1);
    int result = -1;

    std::vector<std::string> order;
    Task t1{"hadamard", Target::QPU, 5, [qid,&order]() {
        order.push_back("h");
        memory.qreg(qid).h(0);
    }};
    Task t2{"measure", Target::QPU, 10, [qid, &result,&order]() {
        order.push_back("m");
        result = memory.qreg(qid).measure(0);
    }};

    scheduler.add_task(t1);
    scheduler.add_task(t2);
    scheduler.run_async();
    scheduler.wait();

    assert(result == 0 || result == 1);
    assert(order.size() == 2);
    assert(order[0] == "m"); // higher priority task first
    memory.release_qregister(qid);

    std::cout << "Scheduler test passed." << std::endl;
    return 0;
}
