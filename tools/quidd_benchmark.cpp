#include "../runtime/wavefunction.h"
#include "../runtime/quidd.h"
#include "../runtime/random.h"
#include <iostream>
#include <random>
#include <vector>
#include <cmath>

int main(int argc, char** argv) {
    using namespace qpp;
    std::size_t qubits = 8;
    if (argc > 1) qubits = std::stoul(argv[1]);

    Wavefunction wf(qubits);
    seed_rng(42);
    std::normal_distribution<double> dist(0.0, 1.0);
    std::vector<std::complex<double>> st(1ULL << qubits);
    for (auto& c : st) c = {dist(global_rng()), dist(global_rng())};
    double norm = 0.0;
    for (const auto& c : st) norm += std::norm(c);
    norm = std::sqrt(norm);
    for (auto& c : st) c /= norm;
    wf.state = st;

    QuIDD dd(wf.state);
    std::size_t wf_bytes = wf.state.size() * sizeof(std::complex<double>);
    std::size_t dd_bytes = dd.memory_bytes();
    std::cout << "Wavefunction bytes: " << wf_bytes << "\n";
    std::cout << "QuIDD bytes: " << dd_bytes << "\n";
    std::cout << "Savings: " << (wf_bytes > dd_bytes ? wf_bytes - dd_bytes : 0) << " bytes\n";
    return 0;
}
