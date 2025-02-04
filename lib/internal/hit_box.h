#ifndef LIB_INTERNAL_HIT_BOX_H
#define LIB_INTERNAL_HIT_BOX_H

#include <vector>

#include "absl/status/statusor.h"
#include "vertex.h"

namespace lib {
namespace internal {

class HitBox {
public:
  HitBox(lib::internal::Vertex center, uint64_t radius) :
      center_(center), radius_(radius), type_(Type::CIRCLE) {}
  static absl::StatusOr<HitBox>
  CreateHitBox(std::vector<lib::internal::Vertex> &&vertices);
  /*
   * TODO(f1lo): Add doc.
   */
  bool Collides(const HitBox &other);

private:
  enum class Type { POINT, LINE, CIRCLE, POLYGON };

  explicit HitBox(std::vector<lib::internal::Vertex> &&vertices, Type type) :
      vertices_(std::move(vertices)), type_(type) {}

  // In case of polygon.
  std::vector<lib::internal::Vertex> vertices_;
  // In case of circle.
  lib::internal::Vertex center_;
  uint64_t radius_;

  Type type_;
};
} // namespace internal
} // namespace lib

#endif // LIB_INTERNAL_HIT_BOX_H
