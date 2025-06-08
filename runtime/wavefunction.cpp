#include "wavefunction.h"
#include <cmath>
#include <random>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace qpp {
// TODO(good-first-issue): consolidate random engine usage across the runtime


template<typename Real>
Wavefunction<Real>::Wavefunction(std::size_t qubits)
    : state(1ULL << qubits, {0.0, 0.0}), num_qubits(qubits) {
    state[0] = Real(1.0);
}

template<typename Real>
static void apply_single_qubit_gate(std::vector<std::complex<Real>>& st,
                                    std::size_t target,
                                    const std::complex<Real> mat[2][2]) {
    std::size_t step = 1ULL << target;
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < st.size(); i += 2 * step) {
#pragma omp simd
        for (std::size_t j = 0; j < step; ++j) {
            auto a = st[i + j];
            auto b = st[i + j + step];
            st[i + j] = mat[0][0] * a + mat[0][1] * b;
            st[i + j + step] = mat[1][0] * a + mat[1][1] * b;
        }
    }
  }
}


template<typename Real>
void Wavefunction<Real>::apply_h(std::size_t qubit) {
    const Real f = Real(1.0) / std::sqrt(Real(2.0));
    const std::complex<Real> mat[2][2] = {{f, f}, {f, -f}};
    apply_single_qubit_gate<Real>(state, qubit, mat);
}

template<typename Real>
void Wavefunction<Real>::apply_x(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {{0, 1}, {1, 0}};
    apply_single_qubit_gate<Real>(state, qubit, mat);
}

template<typename Real>
void Wavefunction<Real>::apply_y(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {
        {Real(0.0), std::complex<Real>(0, -1)},
        {std::complex<Real>(0, 1), Real(0.0)}
    };
    apply_single_qubit_gate<Real>(state, qubit, mat);
}

template<typename Real>
void Wavefunction<Real>::apply_z(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {{1, 0}, {0, -1}};
    apply_single_qubit_gate<Real>(state, qubit, mat);
}

template<typename Real>
void Wavefunction<Real>::apply_s(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {
        {1, 0},
        {0, std::complex<Real>(0, 1)}
    };
    apply_single_qubit_gate<Real>(state, qubit, mat);
}

template<typename Real>
void Wavefunction<Real>::apply_t(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {
        {1, 0},
        {0, std::exp(std::complex<Real>(0, M_PI / 4))}
    };
    apply_single_qubit_gate<Real>(state, qubit, mat);
}

template<typename Real>
void Wavefunction<Real>::apply_swap(std::size_t q1, std::size_t q2) {
    if (q1 == q2) return;
    std::size_t bit1 = 1ULL << q1;
    std::size_t bit2 = 1ULL << q2;
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        bool b1 = i & bit1;
        bool b2 = i & bit2;
        if (b1 != b2) {
            std::size_t j = (i ^ bit1 ^ bit2);
            if (i < j) std::swap(state[i], state[j]);
        }
    }
  }
}


template<typename Real>
void Wavefunction<Real>::apply_cnot(std::size_t control, std::size_t target) {
    std::size_t cbit = 1ULL << control;
    std::size_t tbit = 1ULL << target;
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        if ((i & cbit) && !(i & tbit)) {
            std::size_t j = i | tbit;
            std::swap(state[i], state[j]);
        }
    }
  }
}

template<typename Real>
void Wavefunction<Real>::apply_cz(std::size_t control, std::size_t target) {
    std::size_t cbit = 1ULL << control;
    std::size_t tbit = 1ULL << target;
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        if ((i & cbit) && (i & tbit)) {
            state[i] = -state[i];
        }
    }
}

template<typename Real>
void Wavefunction<Real>::apply_ccnot(std::size_t c1, std::size_t c2, std::size_t target) {
    std::size_t b1 = 1ULL << c1;
    std::size_t b2 = 1ULL << c2;
    std::size_t tbit = 1ULL << target;
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        if ((i & b1) && (i & b2) && !(i & tbit)) {
            std::size_t j = i | tbit;
            std::swap(state[i], state[j]);
        }
    }
  }
}


template<typename Real>
int Wavefunction<Real>::measure(std::size_t qubit) {
    std::size_t bit = 1ULL << qubit;
    double p1 = 0.0;
#pragma omp parallel for reduction(+:p1) schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        if (i & bit)
            p1 += std::norm(state[i]);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(p1);
    int result = dist(gen);
    double norm_factor = std::sqrt(result ? p1 : 1.0 - p1);
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        if (((i & bit) != 0) != static_cast<bool>(result))
            state[i] = 0;
        else
            state[i] /= norm_factor;
    }
    return result;
}

template<typename Real>
std::size_t Wavefunction<Real>::measure(const std::vector<std::size_t>& qubits) {
    if (qubits.empty()) return 0;
    decompress();
    // compute probabilities for all outcomes
    std::size_t outcomes = 1ULL << qubits.size();
    std::vector<double> probs(outcomes, 0.0);
#pragma omp parallel
    {
        std::vector<double> local(outcomes, 0.0);
#pragma omp for schedule(static)
        for (std::size_t i = 0; i < state.size(); ++i) {
            std::size_t outcome = 0;
            for (std::size_t q = 0; q < qubits.size(); ++q) {
                if (i & (1ULL << qubits[q])) outcome |= 1ULL << q;
            }
            local[outcome] += std::norm(state[i]);
        }
#pragma omp critical
        {
            for (std::size_t o = 0; o < outcomes; ++o)
                probs[o] += local[o];
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<std::size_t> dist(probs.begin(), probs.end());
    std::size_t result = dist(gen);
    double norm_factor = std::sqrt(probs[result]);
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        std::size_t outcome = 0;
        for (std::size_t q = 0; q < qubits.size(); ++q) {
            if (i & (1ULL << qubits[q])) outcome |= 1ULL << q;
        }
        if (outcome != result)
            state[i] = 0;
        else
            state[i] /= norm_factor;
    }
    return result;
}

template<typename Real>
void Wavefunction<Real>::reset() {
    state.assign(state.size(), {0.0,0.0});
    if (!state.empty()) state[0] = Real(1.0);
}

template<typename Real>
std::complex<Real> Wavefunction<Real>::amplitude(std::size_t index) const {
    if (index >= state.size()) return {0.0,0.0};
    return state[index];
}

void Wavefunction::compress() {
    if (is_sparse) return;
    sparse_state.clear();
    for (std::size_t i = 0; i < state.size(); ++i) {
        if (std::norm(state[i]) > 1e-12)
            sparse_state[i] = state[i];
    }
    state.clear();
    is_sparse = true;
}

void Wavefunction::decompress() {
    if (!is_sparse) return;
    state.assign(1ULL << num_qubits, {0.0,0.0});
    for (const auto& kv : sparse_state)
        if (kv.first < state.size())
            state[kv.first] = kv.second;
    sparse_state.clear();
    is_sparse = false;
}

std::size_t Wavefunction::nnz() const {
    if (is_sparse) return sparse_state.size();
    std::size_t count = 0;
    for (const auto& amp : state)
        if (std::norm(amp) > 1e-12) ++count;
    return count;
}

// TODO: implement full state collapse for multi-qubit measurements

template class Wavefunction<double>;
template class Wavefunction<float>;

} // namespace qpp
