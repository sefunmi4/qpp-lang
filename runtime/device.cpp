#include "device.h"

namespace qpp {

static DeviceType active_device = DeviceType::CPU;

void set_device(DeviceType d) { active_device = d; }
DeviceType current_device() { return active_device; }

#ifdef USE_CUDA
bool gpu_supported() { return true; }
#else
bool gpu_supported() { return false; }
#endif

} // namespace qpp
