#pragma once
#include <memory>
#include <string>
#include <vector>
namespace qpp {
class QPUBackend {
public:
    virtual ~QPUBackend() = default;
    virtual void execute_qir(const std::string& qir) = 0;
};

void set_qpu_backend(std::unique_ptr<QPUBackend> b);
QPUBackend* qpu_backend();

std::string emit_qir(const std::vector<std::vector<std::string>>& ops);

} // namespace qpp
