#include "../include/runtime_config.h"
#include "../runtime/wavefunction.h"
#include <cassert>
#include <iostream>

int main() {
  using namespace qpp;
  set_disk_limit_mb(1); // trigger disk usage for large states
  Wavefunction big(20); // ~16MB
  assert(big.uses_disk());

  set_disk_limit_mb(64); // disable for next allocation
  Wavefunction small(5);
  assert(!small.uses_disk());

  std::cout << "Disk paging config test passed." << std::endl;
  return 0;
}
