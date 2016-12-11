#include <ctime>
#include <iomanip>
#include <iostream>
#include "Interpolate.hpp"

using namespace cs540;

int main() {
  int i = 1234;
  double x = 3.14;
  std::string str("foo");
  std::time_t t = std::time(nullptr);
  std::tm tm = *std::localtime(&t);
  std::cout << Interpolate(R"(i=%, x1=%, x2=%\%, str1=%, str2=%, obj=%)", i, x,
                           1.001, str, "hello", std::put_time(&tm, "%c %Z"))
            << std::endl;
  std::cout << Interpolate(R"(hello \%)") << std::endl;
}
