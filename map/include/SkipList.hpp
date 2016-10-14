// Copyright 2016 William Jagels
#ifndef MAP_INCLUDE_SKIPLIST_HPP_
#define MAP_INCLUDE_SKIPLIST_HPP_

#include <algorithm>
#include <cstdlib>
#include <list>
#include <random>
#include <stack>
#include <utility>
#include <vector>
#include "./debug.h"

namespace cs540 {
template <typename Key_T, typename Mapped_T, std::size_t height>
class SkipList {
 public:
  struct Node;
  using it_t = typename std::list<Node>::iterator;
  using const_it_t = typename std::list<Node>::const_iterator;
  using r_it_t = typename std::list<Node>::reverse_iterator;
  using ValueType = typename std::pair<Key_T, Mapped_T>;

  SkipList() : nodes{} {
    srand(time(NULL));
    // Bottom level stored in std::list
    this->end_iter = nodes.end();
    this->skips = std::vector<it_t>{height - 1, nodes.end()};
  }
  // SkipList(const SkipList &other);

  std::size_t pick_height(void) const {
    for (std::size_t i = 1; i <= height; i++) {
      if (!(rand() % 2)) {
        return i;
      }
    }
    return height;
  }

  size_t size() const { return this->nodes.size(); }

  std::pair<it_t, bool> insert(const ValueType &isert) {
    auto key = isert.first;
    auto *skips = &this->skips;
    std::stack<it_t> hist;
    std::size_t level = height;
    while (level > 1 && skips->size()) {
      for (std::size_t i = std::min(skips->size(), level); i > 0; i--) {
        it_t ref = skips->at(i - 1);
        if (ref == this->end()) {
          level--;
        } else if (ref->key() == key) {
          return {ref, false};
        } else if (ref->key() < key) {
          skips = &ref->skips;
          hist.push(ref);
          logd("Push %d. i = %zu", ref->key(), i);
          break;
        } else {
          level--;
        }
      }
    }

    if (hist.empty()) {
      // Did not move at all.
      logd("%s", "No express lane :(");
      for (auto it = nodes.begin(); it != this->end(); it++) {
        if (key < it->key()) {
          auto inserted = nodes.emplace(it, isert);
          std::pair<it_t, bool> ret{inserted, true};
          size_t h = pick_height();
          logd("Picked %zu", h);
          if (h == 1) {
            return ret;
          }
          inserted->skips.resize(h - 1, this->end());
          for (size_t i = 0; i < h - 1; i++) {
            auto old = this->skips.at(i);
            logd("Relinking NIL -> %d -> %d", inserted->key(), old->key());
            this->skips.at(i) = inserted;
            inserted->skips.at(i) = old;
          }
          return ret;
        }
      }
      nodes.emplace_back(isert);
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
      if (key < it->key()) {
        inserted = nodes.emplace(it, isert);
        break;
      }
    }
    if (inserted == this->end()) {
      nodes.emplace_back(isert);
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
    while (!hist.empty() && linked < h) {
      auto n = hist.top();
      hist.pop();
      logd("At %d, Link candidates h = %zu, linked = %zu", n->key(), h, linked);
      for (size_t i = linked - 1; i < h - 1 && i < n->level(); i++) {
        auto old = n->skips.at(i);
        logd("Relinking %d -> %d -> %d", n->key(), inserted->key(), old->key());
        n->skips.at(i) = inserted;
        inserted->skips.at(i) = old;
        linked++;
      }
    }
    if (linked < h) {
      while (linked < h) {
        auto old = this->skips.at(linked - 1);
        this->skips.at(linked - 1) = inserted;
        inserted->skips.at(linked - 1) = old;
        logd("Relinking NIL -> %d -> %d", inserted->key(), old->key());
        ++linked;
      }
    }
    return ret;
  }

  it_t find(const Key_T &key) {
    auto *skips = &this->skips;
    std::stack<it_t> hist;
    std::size_t level = height;
    while (level > 1 && skips->size()) {
      for (auto i = std::min(skips->size(), level); i > 0; i--) {
        it_t ref = skips->at(i - 1);
        if (ref == nodes.end()) {
          level--;
        } else if (ref->key() == key) {
          return ref;
        } else if (ref->key() < key) {
          skips = &ref->skips;
          hist.push(ref);
          logd("Push %d. i = %zu", ref->key(), i);
          break;
        } else {
          level--;
        }
      }
    }
    if (hist.empty()) {
      // Did not move at all.
      logd("%s", "No express lane :(");
      for (auto it = nodes.begin(); it != nodes.end(); it++)
        if (key == it->key()) return it;
      return nodes.end();
    } else {
      for (auto it = hist.top(); it != nodes.end(); it++) {
        if (key == it->key()) return it;
      }
    }
    logd("Returning %s", "end");
    return this->end();
  }

  void erase(it_t pos) {}

  it_t begin(void) { return nodes.begin(); }
  it_t end(void) { return nodes.end(); }
  const_it_t begin(void) const { return nodes.cbegin(); }
  const_it_t end(void) const { return nodes.cend(); }
  r_it_t rbegin(void) { return nodes.rbegin(); }
  r_it_t rend(void) { return nodes.rend(); }

  struct Node {
    Node(Key_T key, Mapped_T data) : pair{key, data} {}
    Node(ValueType v) : pair{v} {}

    std::size_t level(void) const { return skips.size(); }
    Key_T key(void) const { return pair.first; }
    Key_T &key(void) { return pair.first; }
    Mapped_T data(void) const { return pair.second; }
    Mapped_T &data(void) { return pair.second; }

    ValueType pair;
    std::vector<it_t> skips;
  };

  std::list<Node> nodes;
  std::vector<it_t> skips;
  it_t end_iter;
};
}  // namespace cs540

#endif  // MAP_INCLUDE_SKIPLIST_HPP_
