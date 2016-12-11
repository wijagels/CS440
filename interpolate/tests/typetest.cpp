#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <type_traits>

// In reality, you'd want to use the "member of" metafunction that I posted earlier,
// but this is just showing you another way of writing a metafunction.
struct A {};
struct B {};
struct C {};

// Matches this "generic" specialization if not A or B.
template <typename T>
struct IsAOrB {
    constexpr static bool value = false;
};

template <>
struct IsAOrB<A> {
    constexpr static bool value = true;
};
template <>
struct IsAOrB<B> {
    constexpr static bool value = true;
};

/*
 * Example using just overloading.
 */

// Base case.
void
foo() {}

template <typename T, typename... Ts>
void foo(const T &, const Ts &... rest);
template <typename... Ts>
void foo(std::ios_base &(*fp)(std::ios_base &), const Ts &...rest);

template <typename T, typename... Ts>
void
foo(const T &, const Ts &... rest) {
    std::cout << "Processing some type: " << typeid(T).name() << std::endl;
    foo(rest...);
}

template <typename... Ts>
void
foo(std::ios_base &(*fp)(std::ios_base &), const Ts &...rest) {
    if (fp == std::boolalpha) {
        std::cout << "Processing std::boolalpha..." << std::endl;
    } else {
        std::cout << "Processing some other \"std::ios_base &(std::ios_base &)\" manipulator..." << std::endl;
    }
    foo(rest...);
}

/*
 * Example using SFINAE.
 */

void
foo2() {}

template <typename T, typename... Ts>
typename std::enable_if<IsAOrB<T>::value>::type
foo2(const T &, const Ts &... rest) {
    std::cout << "Processing value of type A or B..." << std::endl;
    foo2(rest...);
}
template <typename T, typename... Ts>
typename std::enable_if<!IsAOrB<T>::value>::type
foo2(const T &, const Ts &... rest) {
    std::cout << "Processing value of type neither A nor B..." << std::endl;
    foo2(rest...);
}

int
main() {
    foo(1.3, std::boolalpha, std::noboolalpha, (void *) nullptr, 1);
    foo2(A(), C(), B());
}
