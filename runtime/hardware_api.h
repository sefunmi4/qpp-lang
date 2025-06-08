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

class QiskitBackend : public QPUBackend {
public:
    void execute_qir(const std::string& qir) override;
};
  
class CirqBackend : public QPUBackend {
public:
    void execute_qir(const std::string& qir) override;
};

class NvidiaBackend : public QPUBackend {
public:
    void execute_qir(const std::string& qir) override;
};

class QSharpBackend : public QPUBackend {
public:
    void execute_qir(const std::string& qir) override;
};

class BraketBackend : public QPUBackend {
public:
    void execute_qir(const std::string& qir) override;
};

class PsiBackend : public QPUBackend {
public:
    void execute_qir(const std::string& qir) override;
};

void set_qpu_backend(std::unique_ptr<QPUBackend> b);
QPUBackend* qpu_backend();

std::string emit_qir(const std::vector<std::vector<std::string>>& ops);

} // namespace qpp
