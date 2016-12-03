#include <cassert>
#include <iostream>
#include "SharedPtr.hpp"

using cs540::SharedPtr;
void basic_tests_1();

class Base1 {
 protected:
  Base1() : derived_destructor_called(false) { printf("Base1::Base1()\n"); }

 private:
  Base1(const Base1 &);             // Disallow.
  Base1 &operator=(const Base1 &);  // Disallow.
 protected:
  ~Base1() {
    printf("Base1::~Base1()\n");
    assert(derived_destructor_called);
  }

 protected:
  bool derived_destructor_called;
};

class Derived : public Base1 {
  friend int main();
  friend void basic_tests_1();

 private:
  Derived() {}
  Derived(const Derived &);             // Disallow.
  Derived &operator=(const Derived &);  // Disallow.
 public:
  ~Derived() {
    printf("Derived::~Derived()\n");
    derived_destructor_called = true;
  }
  int value;
};

class Base_polymorphic {
 protected:
  Base_polymorphic() { printf("Base_polymorphic::Base_polymorphic()\n"); }

 private:
  Base_polymorphic(const Base_polymorphic &);             // Disallow.
  Base_polymorphic &operator=(const Base_polymorphic &);  // Disallow.
 protected:
  virtual ~Base_polymorphic() {
    printf("Base_polymorphic::~Base_polymorphic()\n");
  }
};

class Derived_polymorphic : public Base_polymorphic {
  friend void basic_tests_1();

 private:
  Derived_polymorphic() {}
  Derived_polymorphic(const Derived_polymorphic &);             // Disallow.
  Derived_polymorphic &operator=(const Derived_polymorphic &);  // Disallow.
};

class Derived2_polymorphic : public Base_polymorphic {
 private:
  Derived2_polymorphic() {}
  Derived2_polymorphic(const Derived2_polymorphic &);             // Disallow.
  Derived2_polymorphic &operator=(const Derived2_polymorphic &);  // Disallow.
};

class Base2 {
 protected:
  Base2() : derived_destructor_called(false) { printf("Base2::Base2()\n"); }

 private:
  Base2(const Base2 &);             // Disallow.
  Base2 &operator=(const Base2 &);  // Disallow.
 protected:
  ~Base2() {
    printf("Base2::~Base2()\n");
    assert(derived_destructor_called);
  }

 protected:
  bool derived_destructor_called;
};

class Derived_mi : public Base1, public Base2 {
  friend void basic_tests_1();

 private:
  Derived_mi() {}
  Derived_mi(const Derived_mi &);             // Disallow.
  Derived_mi &operator=(const Derived_mi &);  // Disallow.
 public:
  ~Derived_mi() {
    printf("Derived_mi::~Derived_mi()\n");
    Base1::derived_destructor_called = true;
    Base2::derived_destructor_called = true;
  }
  int value;
};

class Base1_vi {
 protected:
  Base1_vi() : derived_destructor_called(false) {
    printf("Base1_vi::Base1_vi()\n");
  }

 private:
  Base1_vi(const Base1_vi &);             // Disallow.
  Base1_vi &operator=(const Base1_vi &);  // Disallow.
 protected:
  ~Base1_vi() {
    printf("Base1_vi::~Base1_vi()\n");
    assert(derived_destructor_called);
  }

 protected:
  bool derived_destructor_called;
};

class Base2_vi : public virtual Base1_vi {
 protected:
  Base2_vi() { printf("Base2_vi::Base2_vi()\n"); }

 private:
  Base2_vi(const Base2_vi &);             // Disallow.
  Base2_vi &operator=(const Base2_vi &);  // Disallow.
 protected:
  ~Base2_vi() {
    printf("Base2_vi::~Base2_vi()\n");
    assert(derived_destructor_called);
  }
};

int main() { basic_tests_1(); }

void basic_tests_1() {
  int *n = new int;
  *n = 5;
  SharedPtr<int> ptr{n};
  std::cout << *ptr << std::endl;
  SharedPtr<int> ptr2{ptr};
  std::cout << *ptr2 << std::endl;
  SharedPtr<Base1> ptr3{new Derived{}};
  SharedPtr<Base_polymorphic> ptr4{new Derived_polymorphic{}};
  // std::cout << ptr4 << std::endl;
  //
  {
    SharedPtr<Base2> sp2;
    {
      SharedPtr<Base1> sp1;
      {
        SharedPtr<Derived_mi> sp(new Derived_mi);
        sp1 = sp;
        sp2 = cs540::static_pointer_cast<Base2>(
            cs540::static_pointer_cast<Derived_mi>(sp1));
      }  // Destructor for sp called.
    }    // Destructor for sp1 called.
  }      // Destructor for sp2 called.
}
