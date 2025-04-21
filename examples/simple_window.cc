#include <iostream>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/any_invocable.h"
#include "absl/strings/str_join.h"

class A {
 public:
  virtual void print() = 0;
};

class B : public A {
 public:
  void print() override { std::cout << "B" << std::endl; }
};

class C : public B {
 public:
  void print() override { std::cout << "C" << std::endl; }
  void work(A* x) {
    if (this == x) {
      std::cout << "TRUE" << std::endl;
    } else {
      std::cout << "FALSE" << std::endl;
    }
  };
};

int main() {
  A* b = new B();
  A* c = new C();
  std::cout << ((dynamic_cast<B*>(c) == nullptr) ? "null" : "cast")
            << std::endl;
  std::cout << ((dynamic_cast<B*>(b) == nullptr) ? "null" : "cast")
            << std::endl;
}
