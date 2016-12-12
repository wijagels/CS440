// Copyright 2016 William Jagels

#include <cstring>

#ifndef _DEQUE_HPP_
#define _DEQUE_HPP_

#define INITIAL_SIZE 4
#define GROWTH_FACTOR 2
#define DQPREFIX "Deque_"

inline int mod(int a, int b) {
  // Mathematical modulo function
  // http://stackoverflow.com/a/4003287/1666415
  int ret = a % b;
  if (ret < 0)
    ret += b;
  return ret;
}

#define Deque_DEFINE(t)                                                        \
  struct Deque_##t##_Iterator;                                                 \
  struct Deque_##t {                                                           \
    t *data;                                                                   \
    int sz;                                                                    \
    int capacity;                                                              \
    int start;                                                                 \
    int last;                                                                  \
    char type_name[7 + strlen(#t)];                                            \
    /* Read operations */                                                      \
    t &(*at)(Deque_##t *, int);                                                \
    t &(*front)(Deque_##t *);                                                  \
    t &(*back)(Deque_##t *);                                                   \
    bool (*empty)(Deque_##t *);                                                \
    /* Write operations */                                                     \
    void (*pop_front)(Deque_##t *);                                            \
    void (*pop_back)(Deque_##t *);                                             \
    void (*push_front)(Deque_##t *, const t &);                                \
    void (*push_back)(Deque_##t *, const t &);                                 \
    /* Book keeping */                                                         \
    int (*size)(Deque_##t *);                                                  \
    void (*resize)(Deque_##t *);                                               \
    bool (*comp)(const t &, const t &);                                        \
    /* Destructor */                                                           \
    void (*clear)(Deque_##t *);                                                \
    void (*dtor)(Deque_##t *);                                                 \
    /* Iterators */                                                            \
    Deque_##t##_Iterator (*begin)(Deque_##t *);                                \
    Deque_##t##_Iterator (*end)(Deque_##t *);                                  \
  };                                                                           \
  struct Deque_##t##_Iterator {                                                \
    int pos;                                                                   \
    Deque_##t *dp;                                                             \
    void (*inc)(Deque_##t##_Iterator *);                                       \
    void (*dec)(Deque_##t##_Iterator *);                                       \
    t &(*deref)(Deque_##t##_Iterator *);                                       \
  };                                                                           \
  bool Deque_##t##_Iterator_equal(const Deque_##t##_Iterator &it1,             \
                                  const Deque_##t##_Iterator &it2) {           \
    return it1.pos == it2.pos && it1.dp == it2.dp;                             \
  }                                                                            \
  t &Deque_##t##_Iterator_deref(Deque_##t##_Iterator *ip) {                    \
    return ip->dp->at(ip->dp, ip->pos);                                        \
  }                                                                            \
  void Deque_##t##_Iterator_inc(Deque_##t##_Iterator *ip) { ip->pos++; }       \
  void Deque_##t##_Iterator_dec(Deque_##t##_Iterator *ip) {                    \
    ip->pos--;                                                                 \
  } /* Read operations */                                                      \
  t &Deque_##t##_at(Deque_##t *dp, int i) {                                    \
    assert(i >= 0);                                                            \
    assert(i < dp->sz);                                                        \
    return dp->data[mod((dp->start + i), dp->capacity)];                       \
  }                                                                            \
  t &Deque_##t##_front(Deque_##t *dp) { return dp->at(dp, 0); }                \
  t &Deque_##t##_back(Deque_##t *dp) { return dp->at(dp, dp->sz - 1); }        \
  bool Deque_##t##_empty(Deque_##t *dp) { return dp->sz == 0; }                \
  void Deque_##t##_resize(Deque_##t *dp) {                                     \
    int n = dp->capacity * GROWTH_FACTOR;                                      \
    t *data = (t *)malloc(sizeof(t) * n);                                      \
    for (int i = 0; i < dp->sz; i++) {                                         \
      data[i] = dp->at(dp, i);                                                 \
    }                                                                          \
    free(dp->data);                                                            \
    dp->start = 0;                                                             \
    dp->last = dp->sz;                                                         \
    dp->capacity = n;                                                          \
    dp->data = data;                                                           \
  }                                                                            \
  void Deque_##t##_pop_front(Deque_##t *dp) {                                  \
    assert(dp->sz > 0);                                                        \
    dp->sz--;                                                                  \
    dp->start = mod((dp->start + 1), dp->capacity);                            \
  }                                                                            \
  void Deque_##t##_pop_back(Deque_##t *dp) {                                   \
    assert(dp->sz > 0);                                                        \
    dp->sz--;                                                                  \
    dp->last = mod((dp->last - 1), dp->capacity);                              \
  }                                                                            \
  void Deque_##t##_push_front(Deque_##t *dp, const t &el) {                    \
    if (dp->sz >= dp->capacity) {                                              \
      dp->resize(dp);                                                          \
    }                                                                          \
    dp->start = mod((dp->start - 1), dp->capacity);                            \
    dp->data[dp->start] = el;                                                  \
    dp->sz++;                                                                  \
  }                                                                            \
  void Deque_##t##_push_back(Deque_##t *dp, const t &el) {                     \
    if (dp->sz >= dp->capacity) {                                              \
      dp->resize(dp);                                                          \
    }                                                                          \
    dp->last = mod((dp->last + 1), dp->capacity);                              \
    dp->sz++;                                                                  \
    dp->at(dp, dp->sz - 1) = el;                                               \
  }                                                                            \
  int Deque_##t##_size(Deque_##t *dp) { return dp->sz; }                       \
  void Deque_##t##_clear(Deque_##t *dp) {                                      \
    dp->sz = 0;                                                                \
    dp->start = 0;                                                             \
    dp->last = 0;                                                              \
  }                                                                            \
  void Deque_##t##_Iterator_ctor(Deque_##t##_Iterator *ip, Deque_##t *dp,      \
                                 int pos) {                                    \
    ip->dp = dp;                                                               \
    ip->pos = pos;                                                             \
    ip->deref = &Deque_##t##_Iterator_deref;                                   \
    ip->inc = &Deque_##t##_Iterator_inc;                                       \
    ip->dec = &Deque_##t##_Iterator_dec;                                       \
  }                                                                            \
  Deque_##t##_Iterator Deque_##t##_begin(Deque_##t *dp) {                      \
    Deque_##t##_Iterator it;                                                   \
    Deque_##t##_Iterator_ctor(&it, dp, 0);                                     \
    return it;                                                                 \
  }                                                                            \
  Deque_##t##_Iterator Deque_##t##_end(Deque_##t *dp) {                        \
    Deque_##t##_Iterator it;                                                   \
    Deque_##t##_Iterator_ctor(&it, dp, dp->size(dp));                          \
    return it;                                                                 \
  }                                                                            \
  void Deque_##t##_dtor(Deque_##t *dp) { free(dp->data); }                     \
  void Deque_##t##_ctor(Deque_##t *dp, bool (*comp)(const t &, const t &)) {   \
    dp->type_name[0] = '\0';                                                   \
    strcat(dp->type_name, "Deque_");                                           \
    strcat(dp->type_name, #t);                                                 \
    dp->sz = 0;                                                                \
    dp->capacity = INITIAL_SIZE;                                               \
    dp->start = 0;                                                             \
    dp->last = 0;                                                              \
    dp->data = (t *)malloc(sizeof(t) * dp->capacity);                          \
    dp->at = &Deque_##t##_at;                                                  \
    dp->front = &Deque_##t##_front;                                            \
    dp->back = &Deque_##t##_back;                                              \
    dp->empty = &Deque_##t##_empty;                                            \
    dp->pop_front = &Deque_##t##_pop_front;                                    \
    dp->pop_back = &Deque_##t##_pop_back;                                      \
    dp->push_front = &Deque_##t##_push_front;                                  \
    dp->push_back = &Deque_##t##_push_back;                                    \
    dp->size = &Deque_##t##_size;                                              \
    dp->at = &Deque_##t##_at;                                                  \
    dp->resize = &Deque_##t##_resize;                                          \
    dp->clear = &Deque_##t##_clear;                                            \
    dp->comp = comp;                                                           \
    dp->dtor = &Deque_##t##_dtor;                                              \
    dp->begin = &Deque_##t##_begin;                                            \
    dp->end = &Deque_##t##_end;                                                \
  }                                                                            \
  bool Deque_##t##_equal(Deque_##t &d1, Deque_##t &d2) {                       \
    if (d1.size(&d1) != d2.size(&d2))                                          \
      return false;                                                            \
    auto it2 = d2.begin(&d2);                                                  \
    for (auto it1 = d1.begin(&d1);                                             \
         !Deque_##t##_Iterator_equal(it1, d1.end(&d1));                        \
         it1.inc(&it1), it2.inc(&it2)) {                                       \
      if (d1.comp(it1.deref(&it1), it2.deref(&it2)) ||                         \
          d1.comp(it2.deref(&it2), it1.deref(&it1)))                           \
        return false;                                                          \
    }                                                                          \
    return true;                                                               \
  }

#endif // _DEQUE_HPP_
