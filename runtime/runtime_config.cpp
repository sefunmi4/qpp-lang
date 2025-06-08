#include "runtime_config.h"

namespace qpp {
RuntimeConfig runtime_config;

void set_disk_limit_mb(std::size_t mb) { runtime_config.disk_limit_mb = mb; }
} // namespace qpp
