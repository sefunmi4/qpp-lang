#include "wavefunction.h"
#include <cmath>
#include <random>

namespace qpp {
Wavefunction::Wavefunction() {
    state[0] = 1.0;
    state[1] = 0.0;
}

void Wavefunction::apply_h() {
    auto s0 = state[0];
    auto s1 = state[1];
    constexpr double factor = 1.0 / std::sqrt(2.0);
    state[0] = (s0 + s1) * factor;
    state[1] = (s0 - s1) * factor;
}

void Wavefunction::apply_x() {
    auto tmp = state[0];
    state[0] = state[1];
    state[1] = tmp;
}

int Wavefunction::measure() const {
    double p0 = std::norm(state[0]);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(1.0 - p0);
    return d(gen); // returns 0 or 1
}
} // namespace qpp
