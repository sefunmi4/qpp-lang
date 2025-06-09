#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "wavefunction.h"

namespace qpp {
struct QubitRef {
    std::string reg;
    std::size_t index;
    bool operator==(const QubitRef& other) const {
        return reg == other.reg && index == other.index;
    }
};

struct QubitRefHash {
    std::size_t operator()(const QubitRef& q) const {
        return std::hash<std::string>()(q.reg) ^ (std::hash<std::size_t>()(q.index) << 1);
    }
};

using Partition = std::vector<QubitRef>;

std::vector<Partition> analyze_separable_regions(const std::vector<std::vector<std::string>>& ops);

Wavefunction execute_partitions(const std::vector<std::vector<std::string>>& ops);

}
