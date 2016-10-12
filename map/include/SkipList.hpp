// Copyright 2016 William Jagels
#ifndef MAP_INCLUDE_SKIPLIST_HPP_
#define MAP_INCLUDE_SKIPLIST_HPP_

#include <cstdlib>
#include <list>
#include <random>
#include <stack>
#include <vector>
#include "debug.h"

namespace cs540 {
template <typename T, std::size_t height>
class SkipList {
 public:
  struct Node;
  using it_t = typename std::list<Node>::iterator;
  using const_it_t = typename std::list<Node>::const_iterator;
  using r_it_t = typename std::list<Node>::reverse_iterator;

  SkipList() : nodes{} {
    // Bottom level stored in std::list
    this->end_iter = nodes.end();
    this->skips = std::vector<it_t>{height - 1, nodes.end()};
  }
  SkipList(const SkipList &other);

  std::size_t pick_height(void) const {
    for (std::size_t i = 1; i <= height; i++) {
      if (!(rand() % 2)) {
        return i;
      }
    }
    return height;
  }

  std::pair<it_t, bool> insert(const T &data) {
    logd("Insert %d", data);
    auto skips = this->skips;
    std::stack<it_t> hist;
    std::size_t level = height;
    while (level > 1 && skips.size()) {
      for (std::size_t i = std::min(skips.size(), level); i > 0; i--) {
        it_t ref = skips.at(i - 1);
        if (ref == this->end()) {
          level--;
        } else if (ref->data == data) {
          return {ref, false};
        } else if (ref->data < data) {
          skips = ref->skips;
          hist.push(ref);
          i = std::min(i, skips.size());
          logd("Push %d. i = %zu", ref->data, i);
          if (i == 0) break;  // Prevent underflow
        } else {
          level--;
        }
      }
    }

    if (hist.empty()) {
      // Did not move at all.
      logd("%s", "No express lane :(");
      for (auto it = nodes.begin(); it != this->end(); it++) {
        if (data < it->data) {
          auto inserted = nodes.emplace(it, data);
          std::pair<it_t, bool> ret{inserted, true};
          size_t h = pick_height();
          logd("Picked %zu", h);
          if (h == 1) {
            return ret;
          }
          inserted->skips.resize(h - 1, this->end());
          for (size_t i = 0; i < h - 1; i++) {
            auto old = this->skips.at(i);
            logd("Relinking NIL -> %d -> %d", inserted->data, old->data);
            this->skips.at(i) = inserted;
            inserted->skips.at(i) = old;
          }
          return ret;
        }
      }
      nodes.emplace_back(data);
      it_t inserted = this->end();
      inserted--;
      std::pair<it_t, bool> ret{inserted, true};
      size_t h = pick_height();
      logd("Picked %zu", h);
      if (h == 1) {
        return ret;
      }
      inserted->skips.resize(h - 1, this->end());
      for (size_t i = 0; i < h - 1; i++) {
        auto old = this->skips.at(i);
        this->skips.at(i) = inserted;
        inserted->skips.at(i) = old;
      }
      return ret;
    }
    // Final run
    it_t inserted = this->end();
    for (auto it = hist.top(); it != this->end(); it++) {
      if (data < it->data) {
        inserted = nodes.emplace(it, data);
      }
    }
    if (inserted == this->end()) {
      nodes.emplace_back(data);
      inserted = this->end();
      inserted--;
    }
    std::pair<it_t, bool> ret{inserted, true};
    size_t h = pick_height();
    logd("Picked %zu", h);
    if (h == 1) {
      // Easy, link already made.
      return ret;
    }
    inserted->skips.resize(h - 1, this->end());
    size_t linked = 1;
    while (!hist.empty()) {
      auto n = hist.top();
      hist.pop();
      logd("At %d, Link candidates h = %zu, linked = %zu", n->data, h, linked);
      for (size_t i = linked - 1; i < h - 1 && i < n->level(); i++) {
        auto old = n->skips.at(i);
        logd("Relinking %d -> %d -> %d", n->data, inserted->data, old->data);
        n->skips.at(i) = inserted;
        inserted->skips.at(i) = old;
        linked++;
      }
    }
    return ret;
  }

  it_t find(const T &data) {
    auto skips = this->skips;
    std::stack<it_t> hist;
    std::size_t level = height;
    while (level > 1) {
      for (auto i = std::min(skips.size(), level); i > 0; i--) {
        it_t ref = skips.at(i - 1);
        if (ref == nodes.end()) {
          level--;
        } else if (ref->data == data) {
          return ref;
        } else if (ref->data < data) {
          skips = ref->skips;
          i = std::min(i, skips.size());
          logd("Push %d. i = %zu", ref->data, i);
          if (i == 0) break;
          hist.push(ref);
        } else {
          level--;
        }
      }
    }
    if (hist.empty()) {
      // Did not move at all.
      logd("%s", "No express lane :(");
      for (auto it = nodes.begin(); it != nodes.end(); it++)
        if (data == it->data) return it;
      return nodes.end();
    } else {
      for (auto it = hist.top(); it != nodes.end(); it++) {
        if (data == it->data) return it;
      }
    }
    logd("Returning %s", "end");
    return this->end();
  }

  it_t begin(void) { return nodes.begin(); }
  it_t end(void) { return nodes.end(); }
  const_it_t begin(void) const { return nodes.cbegin(); }
  const_it_t end(void) const { return nodes.cend(); }
  r_it_t rbegin(void) { return nodes.rbegin(); }
  r_it_t rend(void) { return nodes.rend(); }

  struct Node {
    explicit Node(T data) : data{data} {}
    T data;
    std::vector<it_t> skips;

    std::size_t level(void) const { return skips.size(); }
  };

 public:
  std::list<Node> nodes;
  std::vector<it_t> skips;
  it_t end_iter;
};
}  // namespace cs540

#endif  // MAP_INCLUDE_SKIPLIST_HPP_
