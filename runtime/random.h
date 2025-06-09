#pragma once
#include <random>
#include <atomic>
#include <thread>

namespace qpp {
namespace detail {
inline std::atomic<uint64_t>& rng_seed() {
    static std::atomic<uint64_t> seed{std::random_device{}()};
    return seed;
}
} // namespace detail

inline std::mt19937& global_rng() {
    thread_local std::mt19937 gen(detail::rng_seed().load(std::memory_order_relaxed));
    return gen;
}

inline void seed_rng(uint64_t seed) {
    detail::rng_seed().store(seed, std::memory_order_relaxed);
    global_rng().seed(seed);
}

} // namespace qpp
