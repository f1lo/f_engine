#include "lib/internal/object.h"
#include "lib/api/vertex.h"

namespace lib {
namespace internal {

using lib::api::Vertex;

void Object::LogState() {
  for (const auto &[x, y] : vertices_ ) {
    LOG(INFO) << "x: " << x << " y: " << y;
  }
}
}  // namespace internal
}  // namespace lib