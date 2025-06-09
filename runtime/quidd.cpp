#include "quidd.h"
#include <cmath>
#include <cassert>

namespace qpp {

QuIDD::Node* QuIDD::make_terminal(const std::complex<double>& val) {
    TermKey key{val.real(), val.imag()};
    auto it = term_table.find(key);
    if (it != term_table.end()) return it->second;
    auto n = std::make_unique<Node>();
    n->low = n->high = nullptr;
    n->var = 0;
    n->is_terminal = true;
    n->value = val;
    Node* ptr = n.get();
    nodes.push_back(std::move(n));
    term_table[key] = ptr;
    return ptr;
}

QuIDD::Node* QuIDD::make_node(std::size_t var, Node* low, Node* high) {
    NodeKey key{var, low, high};
    auto it = unique_table.find(key);
    if (it != unique_table.end()) return it->second;
    auto n = std::make_unique<Node>();
    n->low = low;
    n->high = high;
    n->var = var;
    n->is_terminal = false;
    Node* ptr = n.get();
    nodes.push_back(std::move(n));
    unique_table[key] = ptr;
    return ptr;
}

QuIDD::Node* QuIDD::build(const std::vector<std::complex<double>>& st,
                          std::size_t start, std::size_t end, std::size_t var) {
    if (var == 0) {
        assert(end - start == 1);
        return make_terminal(st[start]);
    }
    std::size_t mid = start + ((end - start) >> 1);
    Node* low = build(st, start, mid, var - 1);
    Node* high = build(st, mid, end, var - 1);
    if (low == high) return low;
    return make_node(var - 1, low, high);
}

QuIDD::QuIDD(const std::vector<std::complex<double>>& state) {
    qubits = 0;
    std::size_t size = state.size();
    while ((1ULL << qubits) < size) ++qubits;
    root = build(state, 0, size, qubits);
}

void QuIDD::fill(Node* node, std::size_t start, std::size_t end,
                 std::vector<std::complex<double>>& out) const {
    if (!node) return;
    if (node->is_terminal) {
        for (std::size_t i = start; i < end; ++i) out[i] = node->value;
        return;
    }
    std::size_t mid = start + ((end - start) >> 1);
    fill(node->low, start, mid, out);
    fill(node->high, mid, end, out);
}

std::vector<std::complex<double>> QuIDD::to_vector() const {
    std::vector<std::complex<double>> out(1ULL << qubits, {0.0,0.0});
    fill(root, 0, out.size(), out);
    return out;
}

} // namespace qpp
