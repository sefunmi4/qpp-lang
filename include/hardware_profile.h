#pragma once
#include <string>
#include <vector>

namespace qpp {

struct HardwareProfile {
    std::string device_id;
    int max_qubits = 0;
    int max_depth = 0;
    std::vector<std::string> supported_gates;
    int coherence_time_us = 0;
};

bool load_hardware_profile(const std::string& path, HardwareProfile& profile);

}
