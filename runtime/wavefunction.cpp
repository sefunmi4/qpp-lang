#include "wavefunction.h"
#include <cmath>
#include <random>

namespace qpp {
// TODO(good-first-issue): consolidate random engine usage across the runtime

Wavefunction::Wavefunction(std::size_t qubits)
    : state(1ULL << qubits, {0.0, 0.0}), num_qubits(qubits) {
    state[0] = 1.0;
}

static void apply_single_qubit_gate(std::vector<std::complex<double>>& st,
                                    std::size_t target,
                                    const std::complex<double> mat[2][2]) {
    std::size_t step = 1ULL << target;
    for (std::size_t i = 0; i < st.size(); i += 2 * step) {
        for (std::size_t j = 0; j < step; ++j) {
            auto a = st[i + j];
            auto b = st[i + j + step];
            st[i + j] = mat[0][0] * a + mat[0][1] * b;
            st[i + j + step] = mat[1][0] * a + mat[1][1] * b;
        }
    }
}

void Wavefunction::apply_h(std::size_t qubit) {
    const double f = 1.0 / std::sqrt(2.0);
    const std::complex<double> mat[2][2] = {{f, f}, {f, -f}};
    apply_single_qubit_gate(state, qubit, mat);
}

void Wavefunction::apply_x(std::size_t qubit) {
    const std::complex<double> mat[2][2] = {{0, 1}, {1, 0}};
    apply_single_qubit_gate(state, qubit, mat);
}

void Wavefunction::apply_y(std::size_t qubit) {
    const std::complex<double> mat[2][2] = {
        {0.0, std::complex<double>(0, -1)},
        {std::complex<double>(0, 1), 0.0}
    };
    apply_single_qubit_gate(state, qubit, mat);
}

void Wavefunction::apply_z(std::size_t qubit) {
    const std::complex<double> mat[2][2] = {{1, 0}, {0, -1}};
    apply_single_qubit_gate(state, qubit, mat);
}

void Wavefunction::apply_s(std::size_t qubit) {
    const std::complex<double> mat[2][2] = {
        {1, 0},
        {0, std::complex<double>(0, 1)}
    };
    apply_single_qubit_gate(state, qubit, mat);
}

void Wavefunction::apply_t(std::size_t qubit) {
    const std::complex<double> mat[2][2] = {
        {1, 0},
        {0, std::exp(std::complex<double>(0, M_PI / 4))}
    };
    apply_single_qubit_gate(state, qubit, mat);
}

void Wavefunction::apply_swap(std::size_t q1, std::size_t q2) {
    if (q1 == q2) return;
    std::size_t bit1 = 1ULL << q1;
    std::size_t bit2 = 1ULL << q2;
    for (std::size_t i = 0; i < state.size(); ++i) {
        bool b1 = i & bit1;
        bool b2 = i & bit2;
        if (b1 != b2) {
            std::size_t j = (i ^ bit1 ^ bit2);
            if (i < j) std::swap(state[i], state[j]);
        }
    }
}

void Wavefunction::apply_cnot(std::size_t control, std::size_t target) {
    std::size_t cbit = 1ULL << control;
    std::size_t tbit = 1ULL << target;
    for (std::size_t i = 0; i < state.size(); ++i) {
        if ((i & cbit) && !(i & tbit)) {
            std::size_t j = i | tbit;
            std::swap(state[i], state[j]);
        }
    }
}

void Wavefunction::apply_cz(std::size_t control, std::size_t target) {
    std::size_t cbit = 1ULL << control;
    std::size_t tbit = 1ULL << target;
    for (std::size_t i = 0; i < state.size(); ++i) {
        if ((i & cbit) && (i & tbit)) {
            state[i] = -state[i];
        }
    }
}

void Wavefunction::apply_ccnot(std::size_t c1, std::size_t c2, std::size_t target) {
    std::size_t b1 = 1ULL << c1;
    std::size_t b2 = 1ULL << c2;
    std::size_t tbit = 1ULL << target;
    for (std::size_t i = 0; i < state.size(); ++i) {
        if ((i & b1) && (i & b2) && !(i & tbit)) {
            std::size_t j = i | tbit;
            std::swap(state[i], state[j]);
        }
    }
}

int Wavefunction::measure(std::size_t qubit) {
    std::size_t bit = 1ULL << qubit;
    double p1 = 0.0;
    for (std::size_t i = 0; i < state.size(); ++i) {
        if (i & bit)
            p1 += std::norm(state[i]);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(p1);
    int result = dist(gen);
    double norm_factor = std::sqrt(result ? p1 : 1.0 - p1);
    for (std::size_t i = 0; i < state.size(); ++i) {
        if (((i & bit) != 0) != static_cast<bool>(result))
            state[i] = 0;
        else
            state[i] /= norm_factor;
    }
    return result;
}

void Wavefunction::reset() {
    state.assign(state.size(), {0.0,0.0});
    if (!state.empty()) state[0] = 1.0;
}

std::complex<double> Wavefunction::amplitude(std::size_t index) const {
    if (index >= state.size()) return {0.0,0.0};
    return state[index];
}

// TODO: implement full state collapse for multi-qubit measurements

} // namespace qpp

