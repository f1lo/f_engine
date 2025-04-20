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
  C* c = new C();
  c->work(b);
  c->work(c);
}
