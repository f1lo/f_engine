#ifndef LIB_INTERNAL_OBJECT_H
#define LIB_INTERNAL_OBJECT_H

#include <utility>
#include <vector>

#include "lib/api/vertex.h"

namespace lib {
namespace internal {

class Object {
public:
  explicit Object(std::vector<lib::api::Vertex> &&vertices) :
      vertices_(std::move(vertices)) {}
  void LogState();

private:
  std::vector<lib::api::Vertex> vertices_;
};
}  // namespace internal
}  // namespace lib


#endif // LIB_INTERNAL_OBJECT_H
