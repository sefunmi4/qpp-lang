#include "patterns.h"

namespace qpp {

void apply_qft2(QRegister& qr, std::size_t q0, std::size_t q1) {
    qr.h(q1);
    qr.cnot(q1, q0);
    qr.s(q0);
    qr.h(q0);
    qr.swap(q0, q1);
}

void apply_grover2(QRegister& qr, std::size_t q0, std::size_t q1) {
    qr.h(q0);
    qr.h(q1);
    qr.cnot(q1, q0);
    qr.z(q0);
    qr.cnot(q1, q0);
    qr.h(q0);
    qr.h(q1);
}

static bool match_qft2(const std::vector<std::vector<std::string>>& ops,
                       std::size_t pos,
                       std::string& reg,
                       std::string& q0,
                       std::string& q1) {
    if (pos + 5 > ops.size()) return false;
    const auto& a = ops[pos];
    const auto& b = ops[pos + 1];
    const auto& c = ops[pos + 2];
    const auto& d = ops[pos + 3];
    const auto& e = ops[pos + 4];
    if (a.size() == 3 && a[0] == "H" &&
        b.size() == 5 && b[0] == "CNOT" &&
        c.size() == 3 && c[0] == "S" &&
        d.size() == 3 && d[0] == "H" &&
        e.size() == 5 && e[0] == "SWAP") {
        reg = a[1];
        if (b[1] == reg && b[3] == reg && c[1] == reg && d[1] == reg &&
            e[1] == reg && e[3] == reg) {
            q1 = a[2];
            q0 = c[2];
            if (b[2] == q1 && b[4] == q0 && d[2] == q0 &&
                e[2] == q0 && e[4] == q1)
                return true;
        }
    }
    return false;
}

static bool match_grover2(const std::vector<std::vector<std::string>>& ops,
                          std::size_t pos,
                          std::string& reg,
                          std::string& q0,
                          std::string& q1) {
    if (pos + 7 > ops.size()) return false;
    const auto& a = ops[pos];
    const auto& b = ops[pos + 1];
    const auto& c = ops[pos + 2];
    const auto& d = ops[pos + 3];
    const auto& e = ops[pos + 4];
    const auto& f = ops[pos + 5];
    const auto& g = ops[pos + 6];
    if (a.size() == 3 && a[0] == "H" &&
        b.size() == 3 && b[0] == "H" &&
        c.size() == 5 && c[0] == "CNOT" &&
        d.size() == 3 && d[0] == "Z" &&
        e.size() == 5 && e[0] == "CNOT" &&
        f.size() == 3 && f[0] == "H" &&
        g.size() == 3 && g[0] == "H") {
        reg = a[1];
        if (b[1] == reg && c[1] == reg && c[3] == reg && d[1] == reg &&
            e[1] == reg && e[3] == reg && f[1] == reg && g[1] == reg) {
            q0 = a[2];
            q1 = b[2];
            if (c[2] == q1 && c[4] == q0 && d[2] == q0 &&
                e[2] == q1 && e[4] == q0 &&
                f[2] == q0 && g[2] == q1)
                return true;
        }
    }
    return false;
}

void optimize_patterns(std::vector<std::vector<std::string>>& ops) {
    std::vector<std::vector<std::string>> out;
    for (std::size_t i = 0; i < ops.size();) {
        std::string reg, q0, q1;
        if (match_qft2(ops, i, reg, q0, q1)) {
            out.push_back({"QFT2", reg, q0, q1});
            i += 5;
            continue;
        }
        if (match_grover2(ops, i, reg, q0, q1)) {
            out.push_back({"GROVER2", reg, q0, q1});
            i += 7;
            continue;
        }
        out.push_back(ops[i]);
        ++i;
    }
    ops.swap(out);
}

} // namespace qpp
