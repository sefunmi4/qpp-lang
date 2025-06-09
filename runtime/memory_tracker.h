#pragma once
#include <vector>
#include <utility>
#include <chrono>
#include <string>
#include <fstream>

namespace qpp {
struct MemoryTracker {
    bool enabled = false;
    std::chrono::steady_clock::time_point start_time;
    std::vector<std::pair<double, size_t>> samples;

    void start() {
        enabled = true;
        start_time = std::chrono::steady_clock::now();
        samples.clear();
    }

    void stop() { enabled = false; }

    void record(size_t bytes) {
        if (!enabled) return;
        double t = std::chrono::duration<double>(
                       std::chrono::steady_clock::now() - start_time)
                       .count();
        samples.emplace_back(t, bytes);
    }

    bool save_csv(const std::string& path) const {
        std::ofstream ofs(path);
        if (!ofs) return false;
        for (const auto& p : samples) {
            ofs << p.first << ',' << p.second << '\n';
        }
        return true;
    }
};

extern MemoryTracker memory_tracker;
} // namespace qpp
