#ifndef LIB_INTERNAL_HIT_BOX_H
#define LIB_INTERNAL_HIT_BOX_H

#include <memory>
#include <vector>

#include "absl/status/statusor.h"
#include "geometry/shape.h"
#include "lib/api/common_types.h"

namespace lib {
namespace internal {

class HitBox {
 public:
  static HitBox CreateHitBox(api::FPoint point);
  static HitBox CreateHitBox(api::FLine line);
  static HitBox CreateHitBox(api::FRectangle rectangle);
  static HitBox CreateHitBox(api::FCircle circle);

  [[nodiscard]] bool CollidesWith(const HitBox& other) const;
  [[nodiscard]] std::pair<float, float> Reflect(const HitBox& other, float x,
                                                float y) const;
  void Draw() const;
  void Move(const float x, const float y) const { shape_->Move(x, y); }

  float center_x() const { return shape_->center_x(); }
  float center_y() const { return shape_->center_y(); }

 private:
  // TODO(f1lo): Make this type agnostic.
  // When adding new types make sure that `CollidesWith` is appropriately
  // updated.
  enum class ShapeType { POINT, LINE, RECTANGLE, CIRCLE };

  HitBox(std::unique_ptr<Shape> shape, const ShapeType type)
      : shape_(std::move(shape)), shape_type_(type) {}

  std::unique_ptr<Shape> shape_;
  ShapeType shape_type_;
};
}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_HIT_BOX_H
