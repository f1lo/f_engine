#ifndef LIB_INTERNAL_OBJECT_H
#define LIB_INTERNAL_OBJECT_H

#include <utility>
#include <vector>

#include "vertex.h"

namespace lib {
namespace internal {

class Object {
public:
  explicit Object(std::vector<lib::internal::Vertex> &&hit_box_vertices) :
      hit_box_vertices_(std::move(hit_box_vertices)) {}
  void LogState();

private:
  std::vector<lib::internal::Vertex> hit_box_vertices_;
};
} // namespace internal
} // namespace lib


#endif // LIB_INTERNAL_OBJECT_H
