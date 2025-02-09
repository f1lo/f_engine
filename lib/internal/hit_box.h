#ifndef LIB_INTERNAL_HIT_BOX_H
#define LIB_INTERNAL_HIT_BOX_H

#include <memory>
#include <vector>

#include "absl/status/statusor.h"
#include "geometry/shape.h"

namespace lib {
namespace internal {

class HitBox {
public:
  static HitBox CreateHitBox(Point center, const uint32_t radius) {
    return HitBox(std::make_unique<Circle>(Circle{std::move(center), radius}),
                  ShapeType::CIRCLE);
  }
  static absl::StatusOr<HitBox> CreateHitBox(std::vector<Point> &&vertices);

  bool CollidesWith(const HitBox &other) const;

private:
  // TODO(f1lo): Cannot see a better way for now but there has to be. Try to
  // make this type agnostic.
  // When adding new types make sure that `CollidesWith` is appropriately
  // updated.
  enum class ShapeType { POINT, LINE, RECTANGLE, CIRCLE };

  explicit HitBox(std::unique_ptr<Shape> shape, const ShapeType type) :
      shape_(std::move(shape)), shape_type_(type) {}

  std::unique_ptr<Shape> shape_;
  ShapeType shape_type_;
};
} // namespace internal
} // namespace lib

#endif // LIB_INTERNAL_HIT_BOX_H
