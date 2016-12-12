// Copyright 2016 William Jagels

#include <cassert>
#include <cstdlib>
#include <iostream>

#define INITIAL_SIZE 4
#define GROWTH_FACTOR 2

inline int mod(int a, int b) {
  // Mathematical modulo function
  // http://stackoverflow.com/a/4003287/1666415
  int ret = a % b;
  if (ret < 0)
    ret += b;
  return ret;
}

struct Deque_float_Iterator;

struct Deque_float {
  float *data;
  int sz;
  int capacity;
  int start;
  int last;
  /* Read operations */
  float &(*at)(Deque_float *, int);
  float &(*front)(Deque_float *);
  float &(*back)(Deque_float *);
  /* Write operations */
  void (*pop_front)(Deque_float *);
  void (*pop_back)(Deque_float *);
  void (*push_front)(Deque_float *, const float &);
  void (*push_back)(Deque_float *, const float &);
  /* Book keeping */
  int (*size)(Deque_float *);
  void (*resize)(Deque_float *);
  bool (*comp)(const float &, const float &);
  /* Destructor */
  void (*clear)(Deque_float *);
  void (*dtor)(Deque_float *);
  /* Iterators */
  Deque_float_Iterator (*begin)(Deque_float *);
  Deque_float_Iterator (*end)(Deque_float *);
};

struct Deque_float_Iterator {
  int pos;
  Deque_float *dp;
  void (*inc)(Deque_float_Iterator *);
  void (*dec)(Deque_float_Iterator *);
  float &(*deref)(Deque_float_Iterator *);
};

bool Deque_float_Iterator_equal(const Deque_float_Iterator &it1,
                                const Deque_float_Iterator &it2) {
  return it1.pos == it2.pos && it1.dp == it2.dp;
}

float &Deque_float_Iterator_deref(Deque_float_Iterator *ip) {
  return ip->dp->at(ip->dp, ip->pos);
}

void Deque_float_Iterator_inc(Deque_float_Iterator *ip) { ip->pos++; }

void Deque_float_Iterator_dec(Deque_float_Iterator *ip) { ip->pos--; }

/* Read operations */
float &Deque_float_at(Deque_float *dp, int i) {
  assert(i >= 0);
  assert(i < dp->sz);
  return dp->data[mod((dp->start + i), dp->capacity)];
}

float &Deque_float_front(Deque_float *dp) {
  std::cout << dp->start << std::endl;
  return dp->at(dp, 0);
}

float &Deque_float_back(Deque_float *dp) { return dp->at(dp, dp->sz - 1); }

void Deque_float_resize(Deque_float *dp) {
  int n = dp->capacity * GROWTH_FACTOR;
  float *data = (float *)malloc(sizeof(float) * n);
  // Could make this more efficient by using memcpy(3)
  for (int i = 0; i < dp->sz; i++) {
    data[i] = dp->at(dp, i);
  }
  free(dp->data);
  dp->start = 0;
  dp->last = dp->sz;
  dp->capacity = n;
  std::cout << dp->data << " Resized to " << dp->capacity << std::endl;
  dp->data = data;
}

void Deque_float_pop_front(Deque_float *dp) {
  assert(dp->sz > 0);
  dp->sz--;
  dp->start = mod((dp->start + 1), dp->capacity);
}

void Deque_float_pop_back(Deque_float *dp) {
  assert(dp->sz > 0);
  dp->sz--;
  dp->last = mod((dp->last - 1), dp->capacity);
}

void Deque_float_push_front(Deque_float *dp, const float &el) {
  if (dp->sz >= dp->capacity) {
    dp->resize(dp);
  }
  dp->start = mod((dp->start - 1), dp->capacity);
  dp->data[dp->start] = el;
  dp->sz++;
}

void Deque_float_push_back(Deque_float *dp, const float &el) {
  if (dp->sz >= dp->capacity) {
    dp->resize(dp);
  }
  dp->last = mod((dp->last + 1), dp->capacity);
  dp->sz++;
  dp->at(dp, dp->sz - 1) = el;
}

int Deque_float_size(Deque_float *dp) { return dp->sz; }

void Deque_float_clear(Deque_float *dp) {
  dp->sz = 0;
  dp->start = 0;
  dp->last = 0;
}

void Deque_float_Iterator_ctor(Deque_float_Iterator *ip, Deque_float *dp,
                               int pos) {
  ip->dp = dp;
  ip->pos = pos;
  ip->deref = &Deque_float_Iterator_deref;
  ip->inc = &Deque_float_Iterator_inc;
  ip->dec = &Deque_float_Iterator_dec;
}

Deque_float_Iterator Deque_float_begin(Deque_float *dp) {
  Deque_float_Iterator it;
  Deque_float_Iterator_ctor(&it, dp, 0);
  return it;
}

Deque_float_Iterator Deque_float_end(Deque_float *dp) {
  Deque_float_Iterator it;
  Deque_float_Iterator_ctor(&it, dp, dp->size(dp));
  return it;
}

void Deque_float_dtor(Deque_float *dp) { free(dp->data); }

void Deque_float_ctor(Deque_float *dp,
                      bool (*comp)(const float &, const float &)) {
  dp->sz = 0;
  dp->capacity = INITIAL_SIZE;
  dp->start = 0;
  dp->last = 0;
  dp->data = (float *)malloc(sizeof(float) * dp->capacity);
  dp->at = &Deque_float_at;
  dp->front = &Deque_float_front;
  dp->back = &Deque_float_back;
  dp->pop_front = &Deque_float_pop_front;
  dp->pop_back = &Deque_float_pop_back;
  dp->push_front = &Deque_float_push_front;
  dp->push_back = &Deque_float_push_back;
  dp->size = &Deque_float_size;
  dp->at = &Deque_float_at;
  dp->resize = &Deque_float_resize;
  dp->clear = &Deque_float_clear;
  dp->comp = comp;
  dp->dtor = &Deque_float_dtor;
  dp->begin = &Deque_float_begin;
  dp->end = &Deque_float_end;
}

/* End Macro */

bool float_less(const float &o1, const float &o2) { return o1 < o2; }

/*
int main() {
  Deque_float d;
  Deque_float_ctor(&d, float_less);
  std::cout << &d << std::endl;
  d.push_front(&d, 56.1);
  std::cout << d.at(&d, 0) << std::endl;
  d.push_front(&d, 23.1);
  std::cout << d.at(&d, 0) << std::endl;
  d.at(&d, 0) = 3;
  std::cout << d.front(&d) << std::endl; // 3
  d.push_back(&d, 55);
  std::cout << d.back(&d) << std::endl;
  d.push_front(&d, 23.1);
  d.push_front(&d, 24.1);
  d.push_front(&d, 25.1);
  d.pop_back(&d);
  std::cout << d.back(&d) << std::endl;
  d.pop_back(&d);
  std::cout << d.back(&d) << std::endl << std::endl;
  for (auto it = d.begin(&d); !Deque_float_Iterator_equal(it, d.end(&d));
       it.inc(&it)) {
    std::cout << it.deref(&it) << std::endl;
  }
  auto it1 = d.begin(&d);
  auto it2 = d.begin(&d);
  it1.inc(&it1);
  it1.inc(&it1);
  it2.inc(&it2);
  std::cout << Deque_float_Iterator_equal(it1, it2) << std::endl; // 0
  it2.inc(&it2);
  std::cout << Deque_float_Iterator_equal(it1, it2) << std::endl; // 1
  std::cout << it2.deref(&it2) << std::endl;
  it2.deref(&it2) = 42.0;
  std::cout << it2.deref(&it2) << std::endl;
  std::cout << it1.deref(&it1) << std::endl << std::endl;
  d.dtor(&d);
}
*/
