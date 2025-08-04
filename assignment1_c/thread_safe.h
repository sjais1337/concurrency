#pragma once
#include <mutex>
#include <iostream>

struct Shared {
  std::mutex out_mtx;
  std::mutex data_mtx;
  size_t total_occ = 0;
  bool complete = false;
};