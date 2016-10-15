// Copyright 2016 William Jagels
#ifndef MAP_INCLUDE_MAP_HPP_
#define MAP_INCLUDE_MAP_HPP_

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "SkipList.hpp"

using std::size_t;

namespace cs540 {
template <typename Key_T, typename Mapped_T>
class Map {
 public:
  struct Iterator;
  struct ConstIterator;
  struct ReverseIterator;
  typedef std::pair<Key_T, Mapped_T> ValueType;
  Map() : skiplist{} {}
  Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> pairs) {
    for (auto e : pairs) this->insert(e);
  }

  SkipList<Key_T, Mapped_T, 32> skiplist;

  size_t size() const { return this->skiplist.size(); }
  bool empty() const;
  Iterator begin() { return Iterator(this->skiplist.begin()); }
  Iterator end() { return Iterator(this->skiplist.end()); }
  ConstIterator begin() const { return ConstIterator(this->skiplist.begin()); }
  ConstIterator end() const { return ConstIterator(this->skiplist.end()); }
  ReverseIterator rbegin() { return ReverseIterator(this->skiplist.rbegin()); }
  ReverseIterator rend() { return ReverseIterator(this->skiplist.rend()); }
  Iterator find(const Key_T &key) { return this->skiplist.find(key); }
  ConstIterator find(const Key_T &) const;
  Mapped_T &at(const Key_T &tgt) {
    auto f = skiplist.find(tgt);
    if (f == skiplist.end()) throw std::out_of_range("Key not in map");
    return f->data();
  }
  const Mapped_T &at(const Key_T &tgt) const {
    auto f = this->skiplist.find(tgt);
    if (f == skiplist.end()) throw std::out_of_range("Key not in map");
    return f->data();
  }
  Mapped_T &operator[](const Key_T &key) {
    auto r = this->skiplist.find(key);
    if (r != this->skiplist.end()) return r->data();
    ValueType el{key, Mapped_T{}};
    auto iter = this->skiplist.insert(el).first;
    return iter->data();
  }
  std::pair<Iterator, bool> insert(const ValueType &element) {
    return this->skiplist.insert(element);
  }

  template <typename IT_T>
  void insert(IT_T range_beg, IT_T range_end) {
    for (auto it = range_beg; it != range_end; it++) {
      this->insert(*it);
    }
  }

  void erase(Iterator pos) { this->skiplist.erase(pos.iter); }
  void erase(const Key_T &k) {
    auto pos = this->skiplist.find(k);
    if (pos == this->skiplist.end()) throw std::out_of_range("Key not in map");
    this->erase(pos);
  }
  void clear();

  struct Iterator {
    Iterator() = delete;
    typename SkipList<Key_T, Mapped_T, 32>::it_t iter;
    Iterator(const typename SkipList<Key_T, Mapped_T, 32>::it_t &other) {
      this->iter = other;
    }
    Iterator &operator++() {
      ++this->iter;
      return *this;
    }
    Iterator operator++(int) { return this->iter.operator++(0); }
    Iterator &operator--() {
      --this->iter;
      return *this;
    }
    Iterator operator--(int) { return this->iter.operator--(0); }
    ValueType &operator*() const { return this->iter->pair; }
    ValueType *operator->() const { return &this->iter->pair; }
  };

  struct ConstIterator : public SkipList<Key_T, Mapped_T, 32>::const_it_t {
    using parent = typename SkipList<Key_T, Mapped_T, 32>::const_it_t;
    ConstIterator() = delete;
    typename SkipList<Key_T, Mapped_T, 32>::const_it_t iter;
    using value_type = ValueType;
    ConstIterator(
        const typename SkipList<Key_T, Mapped_T, 32>::const_it_t &other)
        : parent{}, iter{other} {}
    ConstIterator(const ConstIterator &other) : parent{}, iter{other.iter} {}
    ConstIterator(const Iterator &other) : parent{}, iter{other.iter} {}
    ConstIterator &operator++() {
      ++this->iter;
      return *this;
    }
    ConstIterator operator++(int) { return this->iter.operator++(0); }
    ConstIterator &operator--() {
      --this->iter;
      return *this;
    }
    ConstIterator operator--(int) { return this->iter.operator--(0); }
    const ValueType &operator*() const { return this->iter->pair; }
    const ValueType *operator->() const { return &this->iter->pair; }
  };

  struct ReverseIterator {
    ReverseIterator() = delete;
    typename SkipList<Key_T, Mapped_T, 32>::r_it_t iter;
    ReverseIterator(const typename SkipList<Key_T, Mapped_T, 32>::r_it_t &other)
        : iter{other} {}
    ReverseIterator &operator++() {
      ++this->iter;
      return *this;
    }
    ReverseIterator operator++(int) { return this->iter.operator++(0); }
    ReverseIterator &operator--() {
      --this->iter;
      return *this;
    }
    ReverseIterator operator--(int) { return this->iter.operator--(0); }
    ValueType &operator*() const { return this->iter->pair; }
    ValueType *operator->() const { return &this->iter->pair; }
  };

  /* Forward iterators */
  friend bool operator==(const Iterator &a, const Iterator &b) {
    return a.iter == b.iter;
  }
  friend bool operator==(const ConstIterator &a, const ConstIterator &b) {
    return a.iter == b.iter;
  }
  friend bool operator==(const Iterator &a, const ConstIterator &b) {
    return a.iter == b.iter;
  }
  friend bool operator==(const ConstIterator &a, const Iterator &b) {
    return a.iter == b.iter;
  }
  /* Inequality */
  friend bool operator!=(const Iterator &a, const Iterator &b) {
    return a.iter != b.iter;
  }
  friend bool operator!=(const ConstIterator &a, const ConstIterator &b) {
    return a.iter != b.iter;
  }
  friend bool operator!=(const Iterator &a, const ConstIterator &b) {
    return a.iter != b.iter;
  }
  friend bool operator!=(const ConstIterator &a, const Iterator &b) {
    return a.iter != b.iter;
  }

  /* Reverse iterator */
  friend bool operator==(const ReverseIterator &a, const ReverseIterator &b) {
    return a.iter == b.iter;
  }
  friend bool operator!=(const ReverseIterator &a, const ReverseIterator &b) {
    return a.iter != b.iter;
  }
};  // class Map

/* Comparisons */
template <typename Key_T, typename Mapped_T>
bool operator==(const Map<Key_T, Mapped_T> &a, const Map<Key_T, Mapped_T> &b) {
  return std::equal(a.begin(), a.end(), b.begin());
}

template <typename Key_T, typename Mapped_T>
bool operator!=(const Map<Key_T, Mapped_T> &a, const Map<Key_T, Mapped_T> &b) {
  return !(a == b);
}

template <typename Key_T, typename Mapped_T>
bool operator<(const Map<Key_T, Mapped_T> &a, const Map<Key_T, Mapped_T> &b) {
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

}  // namespace cs540
#endif  // MAP_INCLUDE_MAP_HPP_
