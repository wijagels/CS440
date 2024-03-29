/*
 * 50 pts:
 *
 * Implement a string which uses *reference* semantics, as opposed to value
 * semantics.  As usual:
 *
 *     - No memory leaks or other memory errors.
 *     - No warnings when compiled with -Wall -Wextra -pedantic.
 *
 * You may put all your code in the header file as inlined functions, or you
 * may put code in String.cpp.
 *
 * You may find the following useful:
 *
 *     std::string::c_str()
 *     strdup()
 *
 * Hint: Internally, you will need to use reference counting.
 *
 */

#include <iostream>
#include <string>
#include "String.hpp"

int main() {
  cs540::String s1;  // Initialize to "".
  // Consider using ctor delegation for these.
  cs540::String s2("hello");
  cs540::String s3(std::string("hello"));

  // After the below, s4 and s1 should point to the same data object.  Also,
  // no memory allocation should occur.
  cs540::String s4(s2);

  cs540::String s5;
  s5 = s2;  // After this, also point to same data object.  Also, no memory
            // allocation should occur.
  s5 = s5;  // Handle self-assignment correctly.

  // Output.
  std::cout << "Output operator, prints \"hello\": " << s2 << std::endl;

  std::cout << "Test reference semantics:" << std::endl;
  // Index.
  s2[0] = 'j';  // Changes to 'j'.
  // Prints "jello".
  std::cout << "Print \"jello\": " << s2 << std::endl;
  // Also prints "jello", since we are using reference semantics.
  std::cout << "Also print \"jello\": " << s5 << std::endl;

  // Index of const.
  const cs540::String &cr{s2};
  std::cout << "Prints 'j': " << cr[0] << std::endl;
  // cr[0] = 'j'; // Must give error if uncommented.

  // Assignment from C string.
  s2 = "hello";
  // Must print "hello".
  std::cout << "Should be \"hello\": " << s5 << std::endl;

  void foo();
  foo();

  // Convert to std::string, implicitly.  Note that, as member functions,
  // conversion operators may also be const, and thus a non-const conversion
  // operator cannot be applied to a const object.
  std::string ss(s5);

  // Concatentation.
  {
    std::cout << "Test concatenation:" << std::endl;
    cs540::String s1("hello");
    cs540::String s2("goodbye");
    std::cout << "Prints \"hello, goodbye\": " << s1 + ", " + s2 << std::endl;
  }
}
