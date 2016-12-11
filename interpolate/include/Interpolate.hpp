#ifndef INTERPOLATE_HPP_
#define INTERPOLATE_HPP_
#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>

namespace cs540 {
constexpr auto ffr(std::ostream &(*f)(std::ostream &)) { return f; }

constexpr auto ffr(std::ios &(*f)(std::ios &)) { return f; }

template <typename... Ta>
class Interpolate_T;

template <typename... Ta>
std::ostream &operator<<(std::ostream &, const Interpolate_T<Ta...> &);

struct WrongNumberOfArgs {};

template <size_t>
struct dummy {};

/*
 * Runtime check for effect of io manipulators
 */
template <typename T>
class IsPut {
 public:
  explicit IsPut(const T &r) {
    std::ostringstream ss{};
    ss << r;
    b_ = ss.tellp() != 0;
  }

  operator bool() const noexcept { return b_; }

 private:
  bool b_;
};

template <typename... Ta>
class Interpolate_T {
 public:
  Interpolate_T(const std::string &fmt_str, const Ta &... args)
      : fmt_str_{fmt_str}, objects_{args...} {}

  std::ostream &print(std::ostream &os) const {
    return print_impl(os, fmt_str_.cbegin(), dummy<0>{});
  }

  template <size_t Index,
            typename = typename std::enable_if<(Index < sizeof...(Ta))>::type>
  std::ostream &print_impl(std::ostream &os, std::string::const_iterator iter,
                           dummy<Index>) const {
    while (iter != fmt_str_.cend()) {
      if (*iter == '\\') {
        if ((iter + 1) != fmt_str_.cend()) {
          if (*(iter + 1) != '%') {
            os << *iter;
          } else {
            os << *(++iter);
          }
        } else {
          os << *iter;
        }
      } else if (*iter == '%') {
        if (insertion(os, std::get<Index>(objects_))) iter++;
        return print_impl(os, iter, dummy<Index + 1>{});
      } else {
        os << *iter;
      }
      iter++;
    }
    if (insertion(os, std::get<Index>(objects_))) {
      // std::cerr << "Loop overrun: " << fmt_str_ << ", "
      //           << std::get<Index>(objects_) << Index << std::endl;
      throw WrongNumberOfArgs{};
    } else {
      return print_impl(os, iter, dummy<Index + 1>{});
    }
  }

  std::ostream &print_impl(std::ostream &os, std::string::const_iterator iter,
                           dummy<sizeof...(Ta)>) const {
    while (iter != fmt_str_.cend()) {
      if (*iter == '\\') {
        if ((iter + 1) != fmt_str_.cend()) {
          if (*(iter + 1) != '%') {
            os << *iter;
          } else {
            os << *(++iter);
          }
        } else {
          os << *iter;
        }
      } else if (*iter == '%') {
        std::cerr << fmt_str_ << std::endl;
        throw WrongNumberOfArgs{};
      } else {
        os << *iter;
      }
      ++iter;
    }
    return os;
  }

  template <typename T>
  bool insertion(std::ostream &os, const T &itm) const {
    os << itm;
    return IsPut<T>(itm);
  }

 private:
  std::string fmt_str_;
  std::tuple<const Ta &...> objects_;
};

template <typename... Ta>
Interpolate_T<Ta...> Interpolate(const std::string &fmt_str,
                                 const Ta &... arguments) {
  return Interpolate_T<Ta...>(fmt_str, arguments...);
}

template <typename... Ta>
std::ostream &operator<<(std::ostream &os, const Interpolate_T<Ta...> &it) {
  return it.print(os);
}

}  // namepace cs540

#endif  // INTERPOLATE_HPP_
