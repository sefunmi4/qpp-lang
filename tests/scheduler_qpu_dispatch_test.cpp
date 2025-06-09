#include "../runtime/scheduler.h"
#include "../runtime/hardware_api.h"
#include <cassert>
#include <iostream>

using namespace qpp;

struct MockBackend : QPUBackend {
    int calls = 0;
    void execute_qir(const std::string& qir) override { ++calls; }
};

int main() {
    auto mock = std::make_unique<MockBackend>();
    MockBackend* ptr = mock.get();
    set_qpu_backend(std::move(mock));

    bool ran = false;
    Task t{"qpu_task", Target::QPU, ExecHint::NONE, 0, [&]() { ran = true; }};
    scheduler.add_task(t);
    scheduler.run();

    assert(ran);
    assert(ptr->calls == 1);
    set_qpu_backend(nullptr);
    std::cout << "Scheduler QPU dispatch test passed." << std::endl;
    return 0;
}
