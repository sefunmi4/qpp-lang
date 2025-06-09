#pragma once
namespace qpp {

enum class DeviceType { CPU, GPU };

void set_device(DeviceType d);
DeviceType current_device();

bool gpu_supported();

} // namespace qpp
