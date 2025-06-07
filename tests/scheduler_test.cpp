#include "../runtime/memory.h"
#include "../runtime/scheduler.h"
#include <iostream>
#include <cassert>

using namespace qpp;

int main() {
    int qid = memory.create_qregister(1);
    int result = -1;

    Task t1{"hadamard", Target::QPU, [qid]() {
        memory.qreg(qid).h(0);
    }};
    Task t2{"measure", Target::QPU, [qid, &result]() {
        result = memory.qreg(qid).measure(0);
    }};

    scheduler.add_task(t1);
    scheduler.add_task(t2);
    scheduler.run();

    assert(result == 0 || result == 1);
    memory.release_qregister(qid);

    std::cout << "Scheduler test passed." << std::endl;
    return 0;
}
