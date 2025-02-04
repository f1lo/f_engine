#include <iostream>

#include "lib//api/vertex.h"
#include "lib/internal/object.h"

using lib::internal::Object;
using lib::api::Vertex;

int main() {
  std::vector<Vertex> vertices;
  vertices.push_back(Vertex{1, 2});
  vertices.push_back(Vertex{3, 4});
  Object o = Object(std::move(vertices));
  o.LogState();
  return 0;
}