#include <iostream>
#include <utility>
#include <vector>
#include "Map.hpp"

int main() {
  using p_t = std::pair<int, int>;
  std::vector<p_t> vec{};
  cs540::Map<int, int> map{};
  cs540::Map<int, int> omap{};
  for (int i = 0; i < 100000; i++) {
    vec.push_back({i, i});
  }
  map.insert<std::vector<p_t>::iterator>(vec.begin(), vec.end());
  for (auto e : map.skiplist.skips) {
    std::cout << e->key() << std::endl;
  }
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 999999);
  for (int i = 100; i < 10000; i++) {
    // map.find(i);
    omap.insert({distribution(generator), 0});
  }
}
