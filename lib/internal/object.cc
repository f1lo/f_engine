#include "lib/internal/object.h"

#include "absl/log/log.h"
#include "vertex.h"

namespace lib {
namespace internal {

using ::lib::internal::Vertex;

void Object::LogState() {
  for (const auto &[x, y]: hit_box_vertices_) {
    LOG(INFO) << "x: " << x << " y: " << y;
  }
}
} // namespace internal
} // namespace lib
