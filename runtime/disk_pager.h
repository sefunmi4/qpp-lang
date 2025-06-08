#pragma once
#include <complex>
#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

namespace qpp {
class DiskPager {
public:
  DiskPager(std::size_t size, std::size_t page_elems = 1024);
  ~DiskPager();

  std::size_t size() const { return total_size; }

  std::complex<double> read(std::size_t idx);
  void write(std::size_t idx, const std::complex<double> &v);
  void flush();
  void reset();

private:
  void load_page(std::size_t page_idx);
  std::fstream file;
  std::string path;
  std::size_t total_size;
  std::size_t page_size;
  std::size_t current_page;
  std::vector<std::complex<double>> buffer;
  bool dirty;
};
} // namespace qpp
