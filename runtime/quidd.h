#pragma once
#include <complex>
#include <unordered_map>
#include <vector>
#include <memory>

namespace qpp {
class QuIDD {
public:
    struct Node {
        Node* low;
        Node* high;
        std::size_t var;
        bool is_terminal;
        std::complex<double> value;
    };

    explicit QuIDD(const std::vector<std::complex<double>>& state);

    std::vector<std::complex<double>> to_vector() const;

    std::size_t node_count() const { return nodes.size(); }
    std::size_t memory_bytes() const { return nodes.size() * sizeof(Node); }

private:
    Node* make_terminal(const std::complex<double>& val);
    Node* make_node(std::size_t var, Node* low, Node* high);
    Node* build(const std::vector<std::complex<double>>& st,
                std::size_t start, std::size_t end, std::size_t var);
    void fill(Node* node, std::size_t start, std::size_t end,
              std::vector<std::complex<double>>& out) const;

    Node* root;
    std::size_t qubits;
    std::vector<std::unique_ptr<Node>> nodes;
    struct NodeKey {
        std::size_t var; Node* low; Node* high;
        bool operator==(const NodeKey& other) const {
            return var==other.var && low==other.low && high==other.high;
        }
    };
    struct NodeKeyHash {
        std::size_t operator()(const NodeKey& k) const {
            return std::hash<std::size_t>()(k.var) ^
                   (std::hash<Node*>()(k.low) << 1) ^
                   (std::hash<Node*>()(k.high) << 2);
        }
    };
    std::unordered_map<NodeKey, Node*, NodeKeyHash> unique_table;
    struct TermKey {
        double r, i;
        bool operator==(const TermKey& o) const { return r==o.r && i==o.i; }
    };
    struct TermKeyHash {
        std::size_t operator()(const TermKey& k) const {
            return std::hash<double>()(k.r) ^ (std::hash<double>()(k.i) << 1);
        }
    };
    std::unordered_map<TermKey, Node*, TermKeyHash> term_table;
};
} // namespace qpp
