#pragma once
#include <shared_mutex>
#include <iostream>

struct Shared {
  std::shared_mutex data_mtx;
  size_t total_occ = 0;
  bool complete = false;
};