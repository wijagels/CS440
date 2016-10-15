#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
#include "Map.hpp"

int main() {
  using p_t = std::pair<int, int>;
  std::vector<p_t> vec{};
  cs540::Map<int, int> map{};
  cs540::Map<int, int> omap{};
  for (int i = 0; i < 100; i++) {
    vec.push_back({i, i});
  }
  map.insert<std::vector<p_t>::iterator>(vec.begin(), vec.end());
  for (auto e : map.skiplist.head_.links) {
    std::cout << e.second->key() << std::endl;
  }
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 999999);
  for (int i = 0; i < 100; i++) {
    // map.find(i);
    // omap.insert({distribution(generator), 0});
    map.erase(i);
    for (auto e : map.skiplist) {
      std::cout << e.key() << std::endl;
      for (auto x : e.links) {
        std::cout << x.first->key() << " -> " << x.second->key() << std::endl;
      }
    }
  }
  cs540::Map<int, int> nmap{};
  auto orig = nmap.insert({4, 2}).first;
  for (auto e : nmap.skiplist) {
    std::cout << e.key() << std::endl;
    for (auto x : e.links) {
      std::cout << x.first->key() << " -> " << x.second->key() << std::endl;
    }
  }
  auto it = nmap.insert({4, 3}).first;
  for (auto e : nmap.skiplist) {
    std::cout << e.key() << std::endl;
    for (auto x : e.links) {
      std::cout << x.first->key() << " -> " << x.second->key() << std::endl;
    }
  }
  std::cout << it->first << std::endl;
  assert(it == orig);
}
