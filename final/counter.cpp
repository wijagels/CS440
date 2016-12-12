/*
 * 60 points
 *
 * For this problem, you are to implement a base class named Counter that can
 * be used to count the number of instances of a class.  The tricky part is
 * making it work under single/multiple inheritance.
 *
 * The usage is similar to a "mix-in" style.  To add counting to a class, you
 * add Counter to the inheritance hierarchy, and make a change to the
 * constructors.  No other changes should be made.
 *
 * The correct output is given in counter_output.txt.
 */

#include <assert.h>
#include <cxxabi.h>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

std::ostream &operator<<(std::ostream &os, const std::type_info &ti) {
  int ec;
  char *p = abi::__cxa_demangle(ti.name(), 0, 0, &ec);
  assert(ec == 0);
  std::string s(p);
  free(p);
  return os << s;
}

static std::unordered_map<std::type_index, size_t> tps_ctor{};
static std::unordered_map<std::type_index, size_t> tps_dtor{};

// You will need to add stuff to this class.
class Counter {
 protected:
  Counter(const std::type_info &ti) : ti{ti} { tps_ctor[ti]++; }
  ~Counter() { tps_dtor[ti]++; }

 public:
  static std::size_t total_constructed(const std::type_info &ti) {
    return tps_ctor[ti];
  }
  static std::size_t total_destructed(const std::type_info &ti) {
    return tps_dtor[ti];
  }
  static std::size_t total_current(const std::type_info &ti) {
    return tps_ctor[ti] - tps_dtor[ti];
  }

 private:
  std::type_index ti;
};

class A : public virtual Counter {
 public:
  A() : Counter{typeid(*this)} {}
};

class B : public A, public virtual Counter {
 public:
  B() : Counter{typeid(*this)} {}
};

class C : public A, public virtual Counter {
 public:
  C() : Counter{typeid(*this)} {}
};

class D : public B, public C, public virtual Counter {
 public:
  D() : Counter{typeid(*this)} {}
};

/*
 * DO NOT CHANGE ANYTHING BELOW!!  WE WILL COPY OVER IT.
 */

void stats(const std::type_info &ti) {
  std::cout << "Type " << ti << ":" << std::endl;
  std::cout << "    " << Counter::total_constructed(ti) << " total constructed."
            << std::endl;
  std::cout << "    " << Counter::total_destructed(ti) << " total destructed."
            << std::endl;
  std::cout << "    " << Counter::total_current(ti)
            << " total currently instantiated." << std::endl;
}

void stats_all() {
  stats(typeid(A));
  stats(typeid(B));
  stats(typeid(C));
  stats(typeid(D));
}

int main() {
  std::cout << "---- Point 1 ----" << std::endl;
  stats_all();

  {
    A a1;
    B b1;
    C c1;
    D d1;
  }

  std::cout << "---- Point 2 ----" << std::endl;
  stats_all();

  {
    A a1, a2;
    B b1, b2;
    C c1, c2;
    D d1, d2;

    A *A_array = new A[10];
    B *B_array = new B[10];
    C *C_array = new C[10];
    D *D_array = new D[10];

    std::cout << "---- Point 3 ----" << std::endl;
    stats_all();

    delete[] A_array;
    delete[] B_array;
    delete[] C_array;
    delete[] D_array;
  }

  std::cout << "---- Point 4 ----" << std::endl;
  stats_all();
}
