// Copyright 2016 William Jagels
#ifndef MAP_INCLUDE_SKIPLIST_HPP_
#define MAP_INCLUDE_SKIPLIST_HPP_

#include <stack>
#include <vector>

namespace cs540 {
template <typename T, std::size_t height>
class SkipList {
 public:
  struct Node;

  SkipList() : skips() {}
  SkipList(const SkipList &other);

  bool insert(const T &data) {
    std::vector<Node *> skips = this->skips;
    std::stack<Node *> hist{};
    while (skips.size()) {
      for (auto it = skips.rbegin(); it != skips.rend(); it++) {
        if ((*it)->data == data) return false;
        if ((*it)->data > data) {
          skips = (*it)->skips;
          hist.push(*it);
        }
      }
    }
    return false;
  }

  const Node *find(const T &data) const {
    std::vector<Node *> skips = this->skips;
    while (skips.size()) {
      for (auto it = skips.rbegin(); it != skips.rend(); it++) {
        if ((*it)->data == data) return *it;
        if ((*it)->data > data) {
          skips = (*it)->skips;
        }
      }
    }
    return nullptr;
  }

  struct Node {
    explicit Node(T data) : data(data) {}
    const T data;
    std::vector<Node *> skips;
    Node *next;
    Node *prev;
  };

  struct Iterator {
    Iterator() = delete;
    Iterator &operator++() {
      ptr = ptr->next;
      return *this;
    }
    Iterator operator++(int) {
      Iterator old = *this;
      ++(*this);
      return old;
    }
    Iterator &operator--() {
      ptr = ptr->prev;
      return *this;
    }
    Iterator operator--(int) {
      Iterator old = *this;
      --(*this);
      return old;
    }
    Node &operator*() const;
    Node *operator->() const;

    Node *ptr;
  };

 private:
  std::vector<Node *> skips;
};
}  // namespace cs540

#endif  // MAP_INCLUDE_SKIPLIST_HPP_
