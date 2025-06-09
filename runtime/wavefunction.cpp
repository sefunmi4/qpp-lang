#include "wavefunction.h"
#include "device.h"
#ifdef USE_CUDA
#include "gpu_kernels.h"
#endif
#include <cmath>
#include <random>
#include "random.h"
#include <unordered_map>
#include <string>
#include <array>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace qpp {
// TODO(good-first-issue): consolidate random engine usage across the runtime


template<typename Real>
Wavefunction<Real>::Wavefunction(std::size_t qubits)
    : state(1ULL << qubits, {Real(0.0), Real(0.0)}), num_qubits(qubits) {
    std::size_t bytes = (1ULL << qubits) * sizeof(std::complex<Real>);
    if (runtime_config.disk_limit_mb > 0 && bytes / (1024 * 1024) >= runtime_config.disk_limit_mb) {
        pager = std::make_unique<DiskPager>(1ULL << qubits);
        disk_backed = true;
        state.clear();
    } else {
        state[0] = Real(1.0);
    }
}

template<typename Real>
static void apply_single_qubit_gate_cpu(std::vector<std::complex<Real>>& st,
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



template<typename Real>
void Wavefunction<Real>::apply_h(std::size_t qubit) {
    const Real f = Real(1.0) / std::sqrt(Real(2.0));
    const std::complex<Real> mat[2][2] = {{f, f}, {f, -f}};
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_x(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {{0, 1}, {1, 0}};
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_y(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {
        {Real(0.0), std::complex<Real>(0, -1)},
        {std::complex<Real>(0, 1), Real(0.0)}
    };
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_z(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {{1, 0}, {0, -1}};
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_s(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {
        {1, 0},
        {0, std::complex<Real>(0, 1)}
    };
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_t(std::size_t qubit) {
    const std::complex<Real> mat[2][2] = {
        {1, 0},
        {0, std::exp(std::complex<Real>(0, M_PI / 4))}
    };
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_rx(std::size_t qubit, Real theta) {
    Real c = std::cos(theta / Real(2.0));
    Real s = std::sin(theta / Real(2.0));
    const std::complex<Real> mat[2][2] = {
        {c, std::complex<Real>(0, -s)},
        {std::complex<Real>(0, -s), c}
    };
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_ry(std::size_t qubit, Real theta) {
    Real c = std::cos(theta / Real(2.0));
    Real s = std::sin(theta / Real(2.0));
    const std::complex<Real> mat[2][2] = {
        {c, -s},
        {s,  c}
    };
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
void Wavefunction<Real>::apply_rz(std::size_t qubit, Real theta) {
    std::complex<Real> e_pos = std::exp(std::complex<Real>(0, theta / Real(2.0)));
    std::complex<Real> e_neg = std::exp(std::complex<Real>(0, -theta / Real(2.0)));
    const std::complex<Real> mat[2][2] = {
        {e_neg, 0},
        {0, e_pos}
    };
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_single_qubit_gate(state, qubit, mat);
#else
        apply_single_qubit_gate_cpu(state, qubit, mat);
#endif
    } else {
        apply_single_qubit_gate_cpu(state, qubit, mat);
    }
}

template<typename Real>
struct Mat2 {
    std::complex<Real> v[2][2];
};

template<typename Real>
static Mat2<Real> mat_mul(const Mat2<Real>& A, const Mat2<Real>& B) {
    Mat2<Real> C{};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            C.v[i][j] = A.v[i][0] * B.v[0][j] + A.v[i][1] * B.v[1][j];
        }
    }
    return C;
}

template<typename Real>
static Mat2<Real> identity_mat() {
    Mat2<Real> I{};
    I.v[0][0] = 1; I.v[0][1] = 0;
    I.v[1][0] = 0; I.v[1][1] = 1;
    return I;
}

template<typename Real>
static Mat2<Real> gate_matrix(const std::string& g) {
    const Real f = Real(1.0) / std::sqrt(Real(2.0));
    if (g == "H") return Mat2<Real>{{{f,f},{f,-f}}};
    if (g == "X") return Mat2<Real>{{{0,1},{1,0}}};
    if (g == "Y") return Mat2<Real>{{{0,std::complex<Real>(0,-1)},{std::complex<Real>(0,1),0}}};
    if (g == "Z") return Mat2<Real>{{{1,0},{0,-1}}};
    if (g == "S") return Mat2<Real>{{{1,0},{0,std::complex<Real>(0,1)}}};
    if (g == "T") return Mat2<Real>{{{1,0},{0,std::exp(std::complex<Real>(0,M_PI/4))}}};
    return identity_mat<Real>();
}

template<typename Real>
void Wavefunction<Real>::apply_fused(const std::vector<std::string>& gates,
                                     std::size_t qubit) {
    if (gates.empty()) return;
    static std::unordered_map<std::string, Mat2<Real>> cache;
    std::string key;
    for (const auto& g : gates) {
        key += g;
        key += ';';
    }
    Mat2<Real> fused;
    auto it = cache.find(key);
    if (it != cache.end()) {
        fused = it->second;
    } else {
        fused = identity_mat<Real>();
        for (const auto& g : gates) {
            fused = mat_mul(gate_matrix<Real>(g), fused);
        }
        cache.emplace(key, fused);
    }
    apply_single_qubit_gate_cpu(state, qubit, fused.v);
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

template<typename Real>
void Wavefunction<Real>::apply_cnot(std::size_t control, std::size_t target) {
    if (current_device() == DeviceType::GPU && gpu_supported()) {
#ifdef USE_CUDA
        gpu_apply_cnot(state, control, target);
#else
        std::size_t cbit = 1ULL << control;
        std::size_t tbit = 1ULL << target;
        for (std::size_t i = 0; i < state.size(); ++i) {
            if ((i & cbit) && !(i & tbit)) {
                std::size_t j = i | tbit;
                std::swap(state[i], state[j]);
            }
        }
#endif
    } else {
        std::size_t cbit = 1ULL << control;
        std::size_t tbit = 1ULL << target;
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


template<typename Real>
int Wavefunction<Real>::measure(std::size_t qubit) {
    std::size_t bit = 1ULL << qubit;
    double p1 = 0.0;
#pragma omp parallel for reduction(+:p1) schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        if (i & bit)
            p1 += std::norm(state[i]);
    }
    std::bernoulli_distribution dist(p1);
    int result = dist(global_rng());
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

    // mask covering all measured qubits
    std::size_t mask = 0;
    for (auto q : qubits) mask |= 1ULL << q;

    // compute probabilities for each measurement outcome
    std::size_t outcomes = 1ULL << qubits.size();
    std::vector<double> probs(outcomes, 0.0);

#pragma omp parallel
    {
        std::vector<double> local(outcomes, 0.0);
#pragma omp for schedule(static)
        for (std::size_t i = 0; i < state.size(); ++i) {
            std::size_t bits = i & mask;
            std::size_t outcome = 0;
            for (std::size_t j = 0; j < qubits.size(); ++j)
                if (bits & (1ULL << qubits[j])) outcome |= 1ULL << j;
            local[outcome] += std::norm(state[i]);
        }
#pragma omp critical
        {
            for (std::size_t o = 0; o < outcomes; ++o)
                probs[o] += local[o];
        }
    }
    std::discrete_distribution<std::size_t> dist(probs.begin(), probs.end());
    std::size_t result = dist(global_rng());
    double norm_factor = std::sqrt(probs[result]);
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < state.size(); ++i) {
        std::size_t bits = i & mask;
        std::size_t outcome = 0;
        for (std::size_t j = 0; j < qubits.size(); ++j)
            if (bits & (1ULL << qubits[j])) outcome |= 1ULL << j;
        if (outcome == result)
            state[i] /= norm_factor;
        else
            state[i] = {Real(0.0), Real(0.0)};
    }

    return result;
}

template<typename Real>
void Wavefunction<Real>::reset() {
    state.assign(state.size(), {Real(0.0),Real(0.0)});
    if (!state.empty()) state[0] = Real(1.0);
}

template<typename Real>
std::complex<Real> Wavefunction<Real>::amplitude(std::size_t index) const {
    if (is_sparse) {
        auto it = sparse_state.find(index);
        if (it == sparse_state.end()) return {Real(0.0), Real(0.0)};
        return it->second;
    }
    if (index >= state.size()) return {Real(0.0),Real(0.0)};
    return state[index];
}

template<typename Real>
bool Wavefunction<Real>::schmidt_low_rank(std::size_t qubit, double threshold) {
    if (qubit >= num_qubits) return false;
    std::size_t mask = 1ULL << qubit;
    std::size_t dim_rest = 1ULL << (num_qubits - 1);
    std::vector<std::complex<Real>> row0(dim_rest), row1(dim_rest);
    std::size_t lowmask = mask - 1;
    std::size_t highmask = (~0ULL) << (qubit + 1);
    for (std::size_t idx = 0; idx < state.size(); ++idx) {
        std::size_t rest = (idx & lowmask) | ((idx & highmask) >> 1);
        if (idx & mask)
            row1[rest] = state[idx];
        else
            row0[rest] = state[idx];
    }
    double n00 = 0.0, n11 = 0.0;
    std::complex<Real> n01{0.0, 0.0};
    for (std::size_t j = 0; j < dim_rest; ++j) {
        n00 += std::norm(row0[j]);
        n11 += std::norm(row1[j]);
        n01 += row0[j] * std::conj(row1[j]);
    }
    double tr = n00 + n11;
    double diff = n00 - n11;
    double root = std::sqrt(diff * diff + 4.0 * std::norm(n01));
    double lambda1 = 0.5 * (tr + root);
    if (lambda1 < 1.0 - threshold)
        return false;

    std::array<std::complex<Real>, 2> u;
    std::complex<Real> x1 = n01;
    std::complex<Real> x2 = lambda1 - n00;
    double norm = std::sqrt(std::norm(x1) + std::norm(x2));
    if (norm < 1e-12) {
        if (n00 >= n11) {
            u = {1.0, 0.0};
        } else {
            u = {0.0, 1.0};
        }
    } else {
        u = {x1 / Real(norm), x2 / Real(norm)};
    }

    std::vector<std::complex<Real>> right(dim_rest);
    for (std::size_t j = 0; j < dim_rest; ++j) {
        right[j] = std::conj(u[0]) * row0[j] + std::conj(u[1]) * row1[j];
    }
    double s0 = std::sqrt(lambda1);
    for (auto& v : right) v /= s0;

    for (std::size_t idx = 0; idx < state.size(); ++idx) {
        std::size_t idx_rest = (idx & lowmask) | ((idx & highmask) >> 1);
        if (idx & mask)
            state[idx] = u[1] * right[idx_rest];
        else
            state[idx] = u[0] * right[idx_rest];
    }
    return true;
}

template<typename Real>
void Wavefunction<Real>::compress() {
    if (is_sparse) return;
    sparse_state.clear();
    for (std::size_t i = 0; i < state.size(); ++i) {
        if (std::norm(state[i]) > 1e-12)
            sparse_state[i] = state[i];
    }
    state.clear();
    is_sparse = true;
}

template<typename Real>
void Wavefunction<Real>::decompress() {
    if (!is_sparse) return;
    state.assign(1ULL << num_qubits, {Real(0.0),Real(0.0)});
    for (const auto& kv : sparse_state)
        if (kv.first < state.size())
            state[kv.first] = kv.second;
    sparse_state.clear();
    is_sparse = false;
}

template<typename Real>
std::size_t Wavefunction<Real>::nnz() const {
    if (is_sparse) return sparse_state.size();
    std::size_t count = 0;
    for (const auto& amp : state)
        if (std::norm(amp) > 1e-12) ++count;
    return count;
}

template<typename Real>
bool Wavefunction<Real>::using_sparse() const {
    return is_sparse;
}

template<typename Real>
std::size_t detect_periodicity_ripple(const Wavefunction<Real>& wf,
                                      double threshold) {
    std::size_t N = wf.using_sparse() ? (1ULL << wf.num_qubits) : wf.state.size();
    if (N < 2)
        return 0;

    std::vector<Real> mags(N, Real(0));
    for (std::size_t i = 0; i < N; ++i) {
        mags[i] = std::abs(wf.amplitude(i));
    }

    std::size_t best_k = 0;
    Real best_amp = Real(0);
    for (std::size_t k = 1; k <= N / 2; ++k) {
        std::complex<Real> sum{0.0, 0.0};
        for (std::size_t n = 0; n < N; ++n) {
            Real angle = -2.0 * M_PI * Real(k) * Real(n) / Real(N);
            std::complex<Real> e{std::cos(angle), std::sin(angle)};
            sum += mags[n] * e;
        }
        Real amp = std::abs(sum);
        if (amp > best_amp) {
            best_amp = amp;
            best_k = k;
        }
    }
    if (best_k == 0)
        return 0;
    Real norm = best_amp / Real(N);
    if (norm < threshold)
        return 0;
    return N / best_k;
}

// TODO: implement full state collapse for multi-qubit measurements

template class Wavefunction<double>;
template class Wavefunction<float>;

template std::size_t detect_periodicity_ripple<double>(const Wavefunction<double>&, double);
template std::size_t detect_periodicity_ripple<float>(const Wavefunction<float>&, double);

} // namespace qpp
