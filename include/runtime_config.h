#pragma once
#include <cstddef>

namespace qpp {
struct RuntimeConfig {
  std::size_t disk_limit_mb = 0; // 0 disables disk paging
};

extern RuntimeConfig runtime_config;
void set_disk_limit_mb(std::size_t mb);
} // namespace qpp
