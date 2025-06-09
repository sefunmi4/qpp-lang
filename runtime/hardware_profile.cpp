#include "hardware_profile.h"
#include <fstream>
#include <regex>
#include <algorithm>

namespace qpp {

bool load_hardware_profile(const std::string& path, HardwareProfile& profile) {
    std::ifstream in(path);
    if (!in.is_open()) return false;
    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::smatch m;
    if (std::regex_search(text, m, std::regex("\"device_id\"\\s*:\\s*\"([^\"]+)\"")))
        profile.device_id = m[1];
    if (std::regex_search(text, m, std::regex("\"max_qubits\"\\s*:\\s*(\\d+)")))
        profile.max_qubits = std::stoi(m[1]);
    if (std::regex_search(text, m, std::regex("\"max_depth\"\\s*:\\s*(\\d+)")))
        profile.max_depth = std::stoi(m[1]);
    if (std::regex_search(text, m, std::regex("\"coherence_time_us\"\\s*:\\s*(\\d+)")))
        profile.coherence_time_us = std::stoi(m[1]);
    std::regex gates_rgx("\"supported_gates\"\\s*:\\s*\\[(.*?)\\]");
    if (std::regex_search(text, m, gates_rgx)) {
        std::string list = m[1];
        std::regex item_rgx("\"([^\"]+)\"");
        auto begin = std::sregex_iterator(list.begin(), list.end(), item_rgx);
        auto end = std::sregex_iterator();
        profile.supported_gates.clear();
        for (auto it = begin; it != end; ++it) profile.supported_gates.push_back((*it)[1]);
    }
    return true;
}

bool check_profile_limits(const HardwareProfile& profile,
                          int qubits,
                          int depth,
                          const std::vector<std::string>& gates,
                          std::ostream& err) {
    bool ok = true;
    if (profile.max_qubits > 0 && qubits > profile.max_qubits) {
        err << "Error: circuit uses " << qubits
            << " qubits but device supports only " << profile.max_qubits
            << std::endl;
        ok = false;
    }
    if (profile.max_depth > 0 && depth > profile.max_depth) {
        err << "Error: circuit depth " << depth
            << " exceeds device max depth " << profile.max_depth << std::endl;
        ok = false;
    }
    if (!profile.supported_gates.empty()) {
        for (const auto& g : gates) {
            if (std::find(profile.supported_gates.begin(),
                          profile.supported_gates.end(), g) ==
                profile.supported_gates.end()) {
                err << "Error: gate '" << g
                    << "' not supported by device" << std::endl;
                ok = false;
            }
        }
    }
    return ok;
}

} // namespace qpp
