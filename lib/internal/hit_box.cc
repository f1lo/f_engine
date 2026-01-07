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

namespace {
/*
 * Constructs convex hull from provided vertices.
 * Returns vertices which are necessary to construct a convex hull.
 */
std::vector<PointInternal> ConstructConvexHull(
    std::vector<PointInternal>& vertices) {
  if (vertices.size() == 1)
    return {vertices[0]};

  std::ranges::sort(vertices,
                    [](const PointInternal& a, const PointInternal& b) {
                      return a.x < b.x || (a.x == b.x && a.y < b.y);
                    });
  const PointInternal point_1 = vertices[0];
  const PointInternal point_2 = vertices.back();
  std::vector<PointInternal> up, down;
  up.push_back(point_1);
  down.push_back(point_1);
  for (size_t i = 1; i < vertices.size(); ++i) {
    if (i == vertices.size() - 1 || Clockwise(point_1, vertices[i], point_2)) {
      while (up.size() >= 2 &&
             !Clockwise(up[up.size() - 2], up[up.size() - 1], vertices[i]))
        up.pop_back();
      up.push_back(vertices[i]);
    }
    if (i == vertices.size() - 1 ||
        CounterClockwise(point_1, vertices[i], point_2)) {
      while (down.size() >= 2 &&
             !CounterClockwise(down[down.size() - 2], down[down.size() - 1],
                               vertices[i]))
        down.pop_back();
      down.push_back(vertices[i]);
    }
  }

  std::vector<PointInternal> hull_vertices;
  for (const auto& i : up)
    hull_vertices.push_back(i);
  for (size_t i = down.size() - 2; i > 0; --i)
    hull_vertices.push_back(down[i]);

  return hull_vertices;
}

/*
 * Checks if the given vertices make a rectangle.
 * Assumes clockwise or counterclockwise order.
 * For now only allows rectangles which are aligned to the X, Y axis.
 */
bool IsAxisAlignedRectangle(const absl::Span<const PointInternal> vertices) {
  if (vertices.size() != 4) {
    return false;
  }

  // Check for dot product between vector of the potential rectangle - if there
  // is 90-degree angle they should be 0.
  //              side_2
  //        |----------------|
  //        |                |
  // side_1 |                | side_3
  //        |----------------|
  // vertices[0]  side_4

  const LineInternal side_1{vertices[0], vertices[1]};
  const LineInternal side_4{vertices[0], vertices[3]};
  const LineInternal side_2{vertices[1], vertices[2]};
  const LineInternal side_3{vertices[3], vertices[2]};

  // Not a rectangle.
  if (side_1.MakeVector().DotProduct(side_4.MakeVector()) != 0 ||
      side_2.MakeVector().DotProduct(side_3.MakeVector()) != 0) {
    return false;
  }

  // All the vector coordinates has to be 0 for the rectangle to be axis
  // aligned.
  return side_1.MakeVector().IsAxisAligned() &&
         side_2.MakeVector().IsAxisAligned() &&
         side_3.MakeVector().IsAxisAligned() &&
         side_4.MakeVector().IsAxisAligned();
}

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

absl::StatusOr<HitBox> HitBox::CreateHitBox(
    std::vector<PointInternal> vertices) {
  if (vertices.empty()) {
    return absl::InvalidArgumentError(
        "empty vertex set while constructing a hit box");
  }
  const size_t old_size = vertices.size();
  const std::vector<PointInternal> normalized_vertices =
      ConstructConvexHull(vertices);
  // Some of the vertices have been discarded.
  if (old_size > normalized_vertices.size()) {
    return absl::InvalidArgumentError(
        "hit box construction failed - some of the hit box vertices have been "
        "discarded. This is due to the fact that vertices where provided in "
        "the wrong order or do not form a convex hull");
  }
  if (old_size < normalized_vertices.size()) {
    return absl::InternalError(
        "THIS SHOULD NEVER HAPPEN - vertices added "
        "while constructing a convex hull.");
  }

  switch (normalized_vertices.size()) {
    case 1:
      return HitBox(std::make_unique<PointInternal>(PointInternal{
                        normalized_vertices[0].x, normalized_vertices[0].y}),
                    ShapeType::POINT);
    case 2:
      return HitBox(
          std::make_unique<LineInternal>(LineInternal{
              PointInternal{normalized_vertices[0].x, normalized_vertices[0].y},
              PointInternal{normalized_vertices[1].x,
                            normalized_vertices[1].y}}),
          ShapeType::LINE);
    case 4:
      // Check if it is a rectangle.
      // TODO(f1lo): Move `IsRectangle` to a rectangle struct and make it a
      // class.
      if (IsAxisAlignedRectangle(normalized_vertices)) {
        return HitBox(
            std::make_unique<RectangleInternal>(RectangleInternal{
                {normalized_vertices[1].x, normalized_vertices[1].y},
                {normalized_vertices[3].x, normalized_vertices[3].y}}),
            ShapeType::RECTANGLE);
      }
      return absl::UnimplementedError(
          "got 4 vertices but no axis aligned rectangle");
    default:
      return absl::UnimplementedError(
          "only points, lines, rectangles and circles are supported");
  }
}  // namespace

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
