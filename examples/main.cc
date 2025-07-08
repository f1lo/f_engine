#include "raylib/include/raylib.h"

#include <iostream>

#include "absl/container/flat_hash_map.h"
#include "absl/strings/str_join.h"

class A {
 public:
  virtual ~A() = default;

  virtual void Print() const = 0;
};

class B : public A {
 public:
  ~B() override { std::cout << "B destroyed" << std::endl; }

  void Print() const override { std::cout << x << "\n"; }

  int x = 0;
};

class D {
 public:
  D(const B* b) : bb(*b) {};

  std::vector<std::unique_ptr<A>> a;
  const B& bb;
};

int main() {
  std::unique_ptr<B> b = std::make_unique<B>();
  b->x++;
  D d(b.get());
  d.bb.Print();
  d.a.push_back(std::move(b));
  d.bb.Print();
}
