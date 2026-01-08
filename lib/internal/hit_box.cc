#include "lib/internal/hit_box.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "absl/log/check.h"
#include "absl/log/log.h"
#include "absl/status/statusor.h"
#include "absl/types/span.h"
#include "geometry/shape.h"

namespace lib {
namespace internal {

using api::FCircle;
using api::FLine;
using api::FPoint;
using api::FRectangle;

namespace {
enum class LineOrientation { ON_LINE, DOWN, UP };

LineOrientation PointInternalOrientation(const float x, const float y,
                                         const std::pair<float, float> line) {
  const float line_y = line.first * x + line.second;
  if (std::abs(line_y - y) <= eps) {
    return LineOrientation::ON_LINE;
  }
  return line_y < y ? LineOrientation::DOWN : LineOrientation::UP;
}

std::pair<float, float> Build45DegreeLine(const float x, const float y) {
  return {-1, x + y};
}

std::pair<float, float> Build135DegreeLine(const float x, const float y) {
  return {1, y - x};
}

std::pair<float, float> ReflectFromRectangle(const RectangleInternal& rec,
                                             const Vector& v,
                                             const float center_x,
                                             const float center_y) {
  const std::pair<float, float> a = Build45DegreeLine(rec.a.x, rec.a.y);
  const std::pair<float, float> b = Build135DegreeLine(rec.b.x, rec.b.y);
  const std::pair<float, float> c = Build45DegreeLine(rec.c.x, rec.c.y);
  const std::pair<float, float> d = Build135DegreeLine(rec.d.x, rec.d.y);
  // Check if the reflection happened on the vertex of a rectangle.
  if (PointInternalOrientation(center_x, center_y, a) ==
          LineOrientation::ON_LINE ||
      PointInternalOrientation(center_x, center_y, b) ==
          LineOrientation::ON_LINE ||
      PointInternalOrientation(center_x, center_y, c) ==
          LineOrientation::ON_LINE ||
      PointInternalOrientation(center_x, center_y, d) ==
          LineOrientation::ON_LINE) {
    return {-v.x, -v.y};
  }

  // Reflected from (rec.a, rec.d) line.
  if (PointInternalOrientation(center_x, center_y, a) ==
          LineOrientation::DOWN &&
      PointInternalOrientation(center_x, center_y, d) ==
          LineOrientation::DOWN) {
    const auto [x, y] = LineInternal(rec.a, rec.d).Reflect(v);
    return {x, y};
  }
  // Reflected from (rec.b, rec.c) line.
  if (PointInternalOrientation(center_x, center_y, b) == LineOrientation::UP &&
      PointInternalOrientation(center_x, center_y, c) == LineOrientation::UP) {
    const auto [x, y] = LineInternal(rec.b, rec.c).Reflect(v);
    return {x, y};
  }
  // Reflected from (rec.a, rec.b) line.
  if (PointInternalOrientation(center_x, center_y, a) == LineOrientation::UP &&
      PointInternalOrientation(center_x, center_y, b) ==
          LineOrientation::DOWN) {
    auto [x, y] = LineInternal(rec.a, rec.b).Reflect(v);
    return {x, y};
  }
  // Reflected from (rec.c, rec.d) line.
  if (PointInternalOrientation(center_x, center_y, c) ==
          LineOrientation::DOWN &&
      PointInternalOrientation(center_x, center_y, d) == LineOrientation::UP) {
    auto [x, y] = LineInternal(rec.c, rec.d).Reflect(v);
    return {x, y};
  }

  // Should be unreachable.
  LOG(WARNING) << rec.a.x << " " << rec.a.y << ", " << rec.b.x << " " << rec.b.y
               << ", " << rec.c.x << " " << rec.c.y << ", " << rec.d.x << " "
               << rec.d.y;
  LOG(WARNING) << center_x << " " << center_y;
  CHECK(false) << "Unreachable code.";
  return {};
}

}  // namespace

HitBox HitBox::CreateHitBox(api::FPoint point) {
  return HitBox(
      std::make_unique<PointInternal>(PointInternal{point.x, point.y}),
      ShapeType::POINT);
}
HitBox HitBox::CreateHitBox(api::FLine line) {
  return HitBox(std::make_unique<LineInternal>(
                    LineInternal{PointInternal{line.a.x, line.a.y},
                                 PointInternal{line.b.x, line.b.y}}),
                ShapeType::LINE);
}
HitBox HitBox::CreateHitBox(api::FRectangle rectangle) {
  return HitBox(
      std::make_unique<RectangleInternal>(RectangleInternal{
          {rectangle.top_left.x, rectangle.top_left.y + rectangle.height},
          {rectangle.top_left.x + rectangle.width, rectangle.top_left.y}}),
      ShapeType::RECTANGLE);
}
HitBox HitBox::CreateHitBox(api::FCircle circle) {
  return HitBox(std::make_unique<CircleInternal>(CircleInternal{
                    {circle.center.x, circle.center.y}, circle.radius}),
                ShapeType::CIRCLE);
}

bool HitBox::CollidesWith(const HitBox& other) const {
  switch (other.shape_type_) {
    // Potentially unsafe if not careful.
    case ShapeType::POINT:
      return this->shape_->Collides(
          *static_cast<PointInternal*>(other.shape_.get()));
    case ShapeType::LINE:
      return this->shape_->Collides(
          *static_cast<LineInternal*>(other.shape_.get()));
    case ShapeType::RECTANGLE:
      return this->shape_->Collides(
          *static_cast<RectangleInternal*>(other.shape_.get()));
    case ShapeType::CIRCLE:
      return this->shape_->Collides(
          *static_cast<CircleInternal*>(other.shape_.get()));
    default:
      // This should never happen.
      // `StatusOr` can be returned here, but it could hurt performance.
      LOG(ERROR) << "HitBox::CollidesWith: invalid shape type";
      return false;
  }
}

std::pair<float, float> HitBox::Reflect(const HitBox& other, const float x,
                                        const float y) const {
  CHECK(shape_type_ == ShapeType::LINE || shape_type_ == ShapeType::RECTANGLE)
      << "Reflection is only implemented from LineInternal or "
         "RectangleInternal.";
  const Vector v = {x, y};
  switch (shape_type_) {
    case ShapeType::LINE: {
      const auto [x, y] = static_cast<LineInternal*>(shape_.get())->Reflect(v);
      return {x, y};
    }
    case ShapeType::RECTANGLE:
      return ReflectFromRectangle(
          *static_cast<RectangleInternal*>(shape_.get()), v,
          other.shape_->center_x(), other.shape_->center_y());
    default:
      CHECK(false) << "HitBox::Reflect: invalid shape type";
      return {};
  }
}

void HitBox::Draw() const {
  shape_->Draw();
}

}  // namespace internal
}  // namespace lib
