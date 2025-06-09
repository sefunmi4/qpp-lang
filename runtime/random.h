#pragma once
#include <random>
#include <atomic>
#include <cstdint>

namespace qpp {
namespace detail {
inline std::atomic<uint64_t>& rng_seed() {
    static std::atomic<uint64_t> seed{std::random_device{}()};
    return seed;
}
} // namespace detail

using rng_engine = std::mt19937_64;

inline rng_engine& global_rng() {
    thread_local rng_engine gen(detail::rng_seed().load(std::memory_order_relaxed));
    return gen;
}

inline void seed_rng(uint64_t seed) {
    detail::rng_seed().store(seed, std::memory_order_relaxed);
    global_rng().seed(seed);
}

} // namespace qpp
