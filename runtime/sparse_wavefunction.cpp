#include "sparse_wavefunction.h"
#include <cmath>
#include <random>
#include "random.h"

namespace qpp {

SparseWavefunction::SparseWavefunction(std::size_t qubits)
    : num_qubits(qubits) {
    state[0] = {1.0, 0.0};
}

void SparseWavefunction::reset() {
    state.clear();
    state[0] = {1.0, 0.0};
}

std::complex<double> SparseWavefunction::amplitude(std::size_t index) const {
    auto it = state.find(index);
    if (it != state.end()) return it->second;
    return {0.0, 0.0};
}

void SparseWavefunction::apply_x(std::size_t qubit) {
    std::unordered_map<std::size_t, std::complex<double>> out;
    std::size_t bit = 1ULL << qubit;
    for (const auto& kv : state) {
        std::size_t flipped = kv.first ^ bit;
        out[flipped] += kv.second;
    }
    state.swap(out);
}

void SparseWavefunction::apply_z(std::size_t qubit) {
    std::size_t bit = 1ULL << qubit;
    for (auto& kv : state) {
        if (kv.first & bit)
            kv.second = -kv.second;
    }
}

void SparseWavefunction::apply_h(std::size_t qubit) {
    std::unordered_map<std::size_t, std::complex<double>> out;
    std::size_t bit = 1ULL << qubit;
    const double f = 1.0 / std::sqrt(2.0);
    for (const auto& kv : state) {
        if (kv.first & bit) {
            std::size_t base = kv.first ^ bit;
            out[base] += f * kv.second;
            out[kv.first] += -f * kv.second;
        } else {
            out[kv.first] += f * kv.second;
            out[kv.first | bit] += f * kv.second;
        }
    }
    state.swap(out);
}

void SparseWavefunction::apply_y(std::size_t qubit) {
	std::unordered_map<std::size_t, std::complex<double>> out;
	std::size_t bit = 1ULL << qubit;
	const auto I = std::complex<double>{0, 1};
	const auto mI = std::complex<double>{0, -1};
	for (auto const& kv : state) {
		auto idx = kv.first;
		if (idx & bit) {
			// basis |1> -> |0> with a -i phase
			out[idx ^ bit] += mI * kv.second;
		} else {
			// basis |0> -> |1> with a +1 phase
			out[idx | bit] += I * kv.second;
		}
	}
	state.swap(out);
}

void SparseWavefunction::apply_s(std::size_t qubit) {
    std::size_t bit = 1ULL << qubit;
    const auto I = std::complex<double>{0, 1};
    for (auto& kv : state) {
        if (kv.first & bit) {
            kv.second *= I;
        }
    }
}

void SparseWavefunction::apply_t(std::size_t qubit) {
    std::size_t bit = 1ULL << qubit;
    const auto phase = std::exp(std::complex<double>{0, M_PI / 4});
    for (auto& kv : state) {
        if (kv.first & bit) {
            kv.second *= phase;
        }
    }
}

void SparseWavefunction::apply_cnot(std::size_t control, std::size_t target) {
    std::unordered_map<std::size_t, std::complex<double>> out;
    std::size_t cbit = 1ULL << control;
    std::size_t tbit = 1ULL << target;
    for (const auto& kv : state) {
        if (kv.first & cbit)
            out[kv.first ^ tbit] += kv.second;
        else
            out[kv.first] += kv.second;
    }
    state.swap(out);
}

int SparseWavefunction::measure(std::size_t qubit) {
    std::size_t bit = 1ULL << qubit;
    double p1 = 0.0;
    for (const auto& kv : state) {
        if (kv.first & bit) p1 += std::norm(kv.second);
    }
    std::bernoulli_distribution dist(p1);
    int result = dist(global_rng());
    double norm_factor = std::sqrt(result ? p1 : 1.0 - p1);
    for (auto it = state.begin(); it != state.end(); ) {
        if ( ((it->first & bit) != 0) != static_cast<bool>(result) ) {
            it = state.erase(it);
        } else {
            it->second /= norm_factor;
            ++it;
        }
    }
    return result;
}

} // namespace qpp

