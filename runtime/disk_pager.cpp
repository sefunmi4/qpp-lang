#include "disk_pager.h"
#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <unistd.h>

namespace qpp {
DiskPager::DiskPager(std::size_t size, std::size_t page_elems)
    : total_size(size), page_size(page_elems),
      current_page(static_cast<std::size_t>(-1)), dirty(false) {
  char tmpl[] = "/tmp/qpp_pagerXXXXXX";
  int fd = mkstemp(tmpl);
  if (fd == -1) {
    throw std::runtime_error("Failed to create temp file for DiskPager");
  }
  close(fd);
  path = tmpl;
  file.open(path,
            std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open temp file for DiskPager");
  }
  buffer.resize(page_size, {0.0, 0.0});
  // initialize file with zeros
  std::vector<std::complex<double>> zeros(page_size, {0.0, 0.0});
  for (std::size_t off = 0; off < size; off += page_size) {
    std::size_t count = std::min(page_size, size - off);
    file.write(reinterpret_cast<char *>(zeros.data()),
               count * sizeof(std::complex<double>));
  }
  file.flush();
}

DiskPager::~DiskPager() {
  flush();
  file.close();
  std::remove(path.c_str());
}

void DiskPager::load_page(std::size_t page_idx) {
  if (current_page == page_idx)
    return;
  flush();
  current_page = page_idx;
  file.seekg(page_idx * page_size * sizeof(std::complex<double>));
  std::size_t count = std::min(page_size, total_size - page_idx * page_size);
  file.read(reinterpret_cast<char *>(buffer.data()),
            count * sizeof(std::complex<double>));
}

void DiskPager::flush() {
  if (current_page == static_cast<std::size_t>(-1) || !dirty)
    return;
  file.seekp(current_page * page_size * sizeof(std::complex<double>));
  std::size_t count =
      std::min(page_size, total_size - current_page * page_size);
  file.write(reinterpret_cast<char *>(buffer.data()),
             count * sizeof(std::complex<double>));
  file.flush();
  dirty = false;
}

std::complex<double> DiskPager::read(std::size_t idx) {
  std::size_t page_idx = idx / page_size;
  load_page(page_idx);
  std::size_t local = idx % page_size;
  return buffer[local];
}

void DiskPager::write(std::size_t idx, const std::complex<double> &v) {
  std::size_t page_idx = idx / page_size;
  load_page(page_idx);
  std::size_t local = idx % page_size;
  buffer[local] = v;
  dirty = true;
}

void DiskPager::reset() {
  flush();
  file.seekp(0);
  std::vector<std::complex<double>> zeros(page_size, {0.0, 0.0});
  for (std::size_t off = 0; off < total_size; off += page_size) {
    std::size_t count = std::min(page_size, total_size - off);
    file.write(reinterpret_cast<char *>(zeros.data()),
               count * sizeof(std::complex<double>));
  }
  file.flush();
  current_page = static_cast<std::size_t>(-1);
  dirty = false;
}

} // namespace qpp
