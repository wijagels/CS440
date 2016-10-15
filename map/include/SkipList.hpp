// Copyright 2016 William Jagels
#ifndef MAP_INCLUDE_SKIPLIST_HPP_
#define MAP_INCLUDE_SKIPLIST_HPP_

#include <algorithm>
#include <cstdlib>
#include <list>
#include <memory>
#include <random>
#include <stack>
#include <utility>
#include <vector>
#include "./debug.h"

namespace cs540 {
template <typename Key_T, typename Mapped_T, size_t height>
class SkipList {
 public:
  struct Node;
  struct Skip;
  using it_t = typename std::list<Node>::iterator;
  using const_it_t = typename std::list<Node>::const_iterator;
  using r_it_t = typename std::list<Node>::reverse_iterator;
  using ValueType = typename std::pair<Key_T, Mapped_T>;

  SkipList() : nodes{} {
    srand(time(NULL));
    // Bottom level stored in std::list
    this->end_iter = nodes.end();
    // this->skips = std::vector<it_t>{height - 1, nodes.end()};
    nodes.push_front(Node());
    this->head_ = nodes.begin();
    this->head_->links =
        std::vector<Skip>{height - 1, {nodes.begin(), nodes.end()}};
    // this->links = std::vector<Skip>{height - 1, {nodes.end(), nodes.end()}};
  }
  // SkipList(const SkipList &other);

  size_t pick_height(void) const {
    for (size_t i = 1; i <= height; i++) {
      if (!(rand() % 2)) {
        return i;
      }
    }
    return height;
  }

  size_t size() const { return this->nodes.size(); }

  void relink(it_t &front, it_t &back, int i) {
    auto old = front->links.at(i).second;
    front->links.at(i).second = back;
    back->links.at(i).first = front;
    back->links.at(i).second = old;
    if (old != this->end()) old->links.at(i).first = back;
  }

  std::pair<it_t, bool> insert(const ValueType &isert) {
    auto key = isert.first;
    auto links = &this->head_->links;
    std::stack<it_t> hist;
    hist.push(this->head_);
    size_t level = height;
    while (level > 1 && links->size()) {
      for (size_t i = std::min(links->size(), level); i > 0; i--) {
        it_t ref = links->at(i - 1).second;
        if (ref == this->end()) {
          level--;
        } else if (ref->key() == key) {
          return {ref, false};
        } else if (ref->key() < key) {
          links = &ref->links;
          hist.push(ref);
          // logd("Push %d. i = %zu", ref->key(), i);
          break;
        } else {
          level--;
        }
      }
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
    // logd("Picked %zu", h);
    if (h == 1) {
      // Easy, link already made.
      return ret;
    }
    inserted->links.resize(h - 1, {this->end(), this->end()});
    size_t linked = 1;
    while (linked < h) {
      auto n = hist.top();
      hist.pop();
      // logd("At %d, Link candidates h = %zu, linked = %zu", n->key(), h,
      // linked);
      for (size_t i = linked - 1; i < h - 1 && i < n->level(); i++) {
        relink(n, inserted, i);
        linked++;
      }
    }
    return ret;
  }

  it_t find(const Key_T &key) {
    auto links = &this->head_->links;
    std::stack<it_t> hist;
    hist.push(this->head_);
    std::size_t level = height;
    while (level > 1 && links->size()) {
      for (auto i = std::min(links->size(), level); i > 0; i--) {
        it_t ref = links->at(i - 1).second;
        if (ref == nodes.end()) {
          level--;
        } else if (ref->key() == key) {
          return ref;
        } else if (ref->key() < key) {
          links = &ref->links;
          hist.push(ref);
          // logd("Push %d. i = %zu", ref->key(), i);
          break;
        } else {
          level--;
        }
      }
    }
    for (auto it = hist.top(); it != nodes.end(); it++) {
      if (key == it->key() && it != this->head_) return it;
    }
    logd("Returning %s", "end");
    return this->end();
  }

  const_it_t find(const Key_T &key) const {
    auto links = &this->head_->links;
    std::stack<it_t> hist;
    hist.push(this->head_);
    std::size_t level = height;
    while (level > 1 && links->size()) {
      for (auto i = std::min(links->size(), level); i > 0; i--) {
        it_t ref = links->at(i - 1).second;
        if (ref == nodes.end()) {
          level--;
        } else if (ref->key() == key && ref != this->head_) {
          return ref;
        } else if (ref->key() < key) {
          links = &ref->links;
          hist.push(ref);
          // logd("Push %d. i = %zu", ref->key(), i);
          break;
        } else {
          level--;
        }
      }
    }
    for (auto it = hist.top(); it != nodes.end(); it++) {
      if (key == it->key() && it != this->head_) return it;
    }
    logd("Returning %s", "end");
    return this->end();
  }

  void erase(it_t &pos) {
    if (pos == this->end()) throw std::out_of_range("Erase failure");
    if (pos == this->head_) throw std::out_of_range("Erase failure");
    for (size_t i = 0; i < pos->links.size(); i++) {
      if (pos->links.at(i).second == this->end()) {
        logd("Bypassing link from %d -> %d -> %s",
             pos->links.at(i).first->key(), pos->key(), "NIL");
      }
      logd("Bypassing link from %d -> %d -> %d", pos->links.at(i).first->key(),
           pos->key(), pos->links.at(i).second->key());
      auto &front = pos->links.at(i).first;
      auto &back = pos->links.at(i).second;
      front->links.at(i).second = back;
      if (back != this->end()) back->links.at(i).first = front;
    }
    logd("Erasing %d", pos->key());
    this->nodes.erase(pos);
  }

  it_t begin(void) { return nodes.begin(); }
  it_t end(void) { return nodes.end(); }
  const_it_t begin(void) const { return nodes.cbegin(); }
  const_it_t end(void) const { return nodes.cend(); }
  r_it_t rbegin(void) { return nodes.rbegin(); }
  r_it_t rend(void) { return nodes.rend(); }

  struct Sentinel {
    Sentinel() : links{} {}
    Sentinel(const Node &n) : links{n.links} {}

    size_t level(void) const { return links.size(); }

    std::vector<Skip> links;
  };

  struct Node : Sentinel {
    Node() : Sentinel() {}
    Node(Key_T key, Mapped_T data) : Sentinel(), pair{key, data} {}
    Node(ValueType v) : Sentinel(), pair{v} {}

    // size_t level(void) const { return links.size(); }
    const Key_T &key(void) const { return pair.first; }
    Key_T &key(void) { return pair.first; }
    const Mapped_T &data(void) const { return pair.second; }
    Mapped_T &data(void) { return pair.second; }

    ValueType pair;
  };

  struct Skip {
    Skip(const it_t &first, const it_t &second)
        : first{first}, second{second} {}
    it_t first;
    it_t second;
  };

  std::list<Node> nodes;
  it_t head_;
  it_t end_iter;
};
}  // namespace cs540

#endif  // MAP_INCLUDE_SKIPLIST_HPP_
