// Copyright 2016 William Jagels
#ifndef MAP_INCLUDE_MAP_HPP_
#define MAP_INCLUDE_MAP_HPP_

#include <initializer_list>
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
  Map();
  Map(const Map &);
  Map &operator=(const Map &);
  Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> pairs) {
    for (auto e : pairs) this->insert(e);
  }
  ~Map();

  SkipList<ValueType, 32> skiplist;

  size_t size() const;
  bool empty() const;
  Iterator begin() { return Iterator(this->skiplist.begin()); }
  Iterator end() { return Iterator(this->skiplist.end()); }
  ConstIterator begin() const { return ConstIterator(this->skiplist.begin()); }
  ConstIterator end() const { return ConstIterator(this->skiplist.end()); }
  ReverseIterator rbegin();
  ReverseIterator rend();
  Iterator find(const Key_T &);
  ConstIterator find(const Key_T &) const;
  Mapped_T &at(const Key_T &tgt) {
    auto f = skiplist.find(tgt);
    return f->data.second;
  }
  const Mapped_T &at(const Key_T &) const;
  Mapped_T &operator[](const Key_T &);
  std::pair<Iterator, bool> insert(const ValueType &);

  template <typename IT_T>
  void insert(IT_T range_beg, IT_T range_end);

  void erase(Iterator pos);
  void erase(const Key_T &);
  void clear();
  bool operator==(const Map &);
  bool operator!=(const Map &);
  bool operator<(const Map &);

  struct Iterator {
    Iterator() = delete;
    typename SkipList<ValueType, 32>::it_t iter;
    Iterator(typename SkipList<ValueType, 32>::it_t other) {
      this->iter = other;
    }
    // Iterator(const Iterator &);
    // Iterator(const super &other) : super{} {}
    // Iterator &operator=(const Iterator &);
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
    ValueType &operator*() const { return this->iter->data; }
    // ValueType *operator->() const { return super->operator->(); }
  };
  struct ConstIterator {
    ConstIterator() = delete;
    typename SkipList<ValueType, 32>::const_it_t iter;
    ConstIterator(typename SkipList<ValueType, 32>::const_it_t other) {
      this->iter = other;
    }
    // ConstIterator(const ConstIterator &);
    // ConstIterator(const Iterator &);
    // ConstIterator &operator=(const ConstIterator &);
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
    const ValueType &operator*() const { return this->iter->data; }
    const ValueType *operator->() const { return this->iter->data; }
  };
  struct ReverseIterator : public Iterator {
    ReverseIterator() = delete;
    ReverseIterator(const ReverseIterator &);
    ReverseIterator &operator=(const ReverseIterator &);
    ReverseIterator &operator++();
    ReverseIterator operator++(int);
    ReverseIterator &operator--();
    ReverseIterator operator--(int);
    ValueType &operator*() const;
    ValueType *operator->() const;
  };
  friend bool operator==(const Iterator &, const Iterator &) { return false; }
  friend bool operator==(const ConstIterator &, const ConstIterator &) {
    return false;
  }
  friend bool operator==(const Iterator &, const ConstIterator &) {
    return false;
  }
  friend bool operator==(const ConstIterator &, const Iterator &) {
    return false;
  }
  friend bool operator!=(const Iterator &, const Iterator &) { return false; }
  friend bool operator!=(const ConstIterator &, const ConstIterator &) {
    return false;
  }
  friend bool operator!=(const Iterator &, const ConstIterator &) {
    return false;
  }
  friend bool operator!=(const ConstIterator &, const Iterator &) {
    return false;
  }
  friend bool operator==(const ReverseIterator &, const ReverseIterator &) {
    return false;
  }
  friend bool operator!=(const ReverseIterator &, const ReverseIterator &) {
    return false;
  }
};
}  // namespace cs540
#endif  // MAP_INCLUDE_MAP_HPP_
