#pragma once
#include <string>
#include <vector>
#include <ostream>

namespace qpp {

struct HardwareProfile {
    std::string device_id;
    int max_qubits = 0;
    int max_depth = 0;
    std::vector<std::string> supported_gates;
    int coherence_time_us = 0;
};

bool load_hardware_profile(const std::string& path, HardwareProfile& profile);

// Validate a circuit against the provided hardware profile. Returns true if the
// circuit fits within all limits, otherwise prints an error message to `err`
// and returns false.
bool check_profile_limits(const HardwareProfile& profile,
                          int qubits,
                          int depth,
                          const std::vector<std::string>& gates,
                          std::ostream& err);

}
