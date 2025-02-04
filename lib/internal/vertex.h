#ifndef LIB_INTERNAL_VERTEX_H
#define LIB_INTERNAL_VERTEX_H

namespace lib {
namespace internal {

struct Vertex {
  int x;
  int y;

  /*
   * Returns true if current vertex is located on the lower-left side of the
   * `other` vertex. Does not return `true` if they are the same points.
   */
  [[nodiscard]] bool IsLowerLeft(const Vertex &other) const;
};

} // namespace internal
} // namespace lib

#endif // LIB_INTERNAL_VERTEX_H
