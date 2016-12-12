/*
 * 40 points
 *
 * For this problem, you must implement Subtract() as a variadic
 * function template.  There will at least one overload, since you need a base
 * case.  Note that the most obvious way of implementing it will use the wrong
 * operation order.
 *
 * Hint: "Peel off" more than one argument at a time.  The rest of the
 * arguments are passed as the pack.
 */

#include <assert.h>
#include <iostream>
#include <type_traits>

// Do not modify any of the classes below.
struct A {};
struct B {};
struct C {};
struct D {};
struct E {};

D operator-(const A &, const B &) { return D(); }
E operator-(const D &, const C &) { return E(); }

std::ostream &operator<<(std::ostream &os, const E &) {
  return os << "[E object]";
}

// Put your Subtract implementation here.

template <typename T>
constexpr auto Subtract(T first) {
  return first;
}

template <typename T, typename U, typename... Ts>
constexpr auto Subtract(T first, U second, Ts... other) {
  return Subtract((first - second), other...);
}

// Do not modify the code below.
int main() {
  // Should print 1
  std::cout << Subtract(4, 1, 2) << std::endl;
  // Should print 1.1
  std::cout << Subtract(4.1, 1, 2) << std::endl;
  // Should output just "[E object]".
  std::cout << Subtract(A(), B(), C()) << std::endl;
}
