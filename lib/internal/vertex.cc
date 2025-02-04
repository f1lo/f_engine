#include "lib/internal/vertex.h"

namespace lib {
namespace internal {

bool Vertex::IsLowerLeft(const Vertex &other) const {
  return this->x <= other.x && this->y < other.y;
}

} // namespace internal
} // namespace lib
