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
  Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>);
  ~Map();
  size_t size() const;
  bool empty() const;
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;
  ReverseIterator rbegin();
  ReverseIterator rend();
  Iterator find(const Key_T &);
  ConstIterator find(const Key_T &) const;
  Mapped_T &at(const Key_T &);
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
    Iterator(const Iterator &);
    Iterator &operator=(const Iterator &);
    Iterator &operator++();
    Iterator operator++(int);
    Iterator &operator--();
    Iterator operator--(int);
    ValueType &operator*() const;
    ValueType *operator->() const;
  };
  struct ConstIterator {
    ConstIterator() = delete;
    ConstIterator(const ConstIterator &);
    ConstIterator &operator=(const ConstIterator &);
    ConstIterator &operator++();
    ConstIterator operator++(int);
    ConstIterator &operator--();
    ConstIterator operator--(int);
    ValueType &operator*() const;
    ValueType *operator->() const;
  };
  struct ReverseIterator {
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
