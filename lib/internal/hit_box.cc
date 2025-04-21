#include "lib/internal/hit_box.h"

#include <memory>
#include <vector>

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
std::vector<Point> ConstructConvexHull(std::vector<Point> vertices) {
  if (vertices.size() == 1)
    return {vertices[0]};

  std::sort(vertices.begin(), vertices.end(),
            [](const Point& a, const Point& b) {
              return a.x < b.x || (a.x == b.x && a.y < b.y);
            });
  Point point_1 = vertices[0];
  Point point_2 = vertices.back();
  std::vector<Point> up, down;
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

  std::vector<Point> hull_vertices;
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
bool IsAxisAlignedRectangle(absl::Span<const Point> vertices) {
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

  const Line side_1{vertices[0], vertices[1]};
  const Line side_4{vertices[0], vertices[3]};
  const Line side_2{vertices[1], vertices[2]};
  const Line side_3{vertices[3], vertices[2]};

  // Not a rectangle.
  if (side_1.MakeVector().ScalarProduct(side_4.MakeVector()) != 0 ||
      side_2.MakeVector().ScalarProduct(side_3.MakeVector()) != 0) {
    return false;
  }

  // All the vector coordinates has to be 0 for the rectangle to be axis
  // aligned.
  return side_1.MakeVector().IsAxisAligned() &&
         side_2.MakeVector().IsAxisAligned() &&
         side_3.MakeVector().IsAxisAligned() &&
         side_4.MakeVector().IsAxisAligned();
}
}  // namespace

absl::StatusOr<HitBox> HitBox::CreateHitBox(std::vector<Point>&& vertices) {
  if (vertices.empty()) {
    return absl::InvalidArgumentError(
        "empty vertex set while constructing a hit box");
  }
  const size_t old_size = vertices.size();
  const std::vector<Point> normalized_vertices =
      ConstructConvexHull(std::move(vertices));
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
      return HitBox(std::make_unique<Point>(Point{normalized_vertices[0].x,
                                                  normalized_vertices[0].y}),
                    ShapeType::POINT);
    case 2:
      return HitBox(
          std::make_unique<Line>(
              Line{Point{normalized_vertices[0].x, normalized_vertices[0].y},
                   Point{normalized_vertices[1].x, normalized_vertices[1].y}}),
          ShapeType::LINE);
    case 4:
      // Check if it is a rectangle.
      // TODO(f1lo): Move `IsRectangle` to a rectangle struct and make it a
      // class.
      if (IsAxisAlignedRectangle(normalized_vertices)) {
        return HitBox(
            std::make_unique<Rectangle>(Rectangle{
                {normalized_vertices[0].x, normalized_vertices[0].y},
                {normalized_vertices[1].x, normalized_vertices[1].y},
                {normalized_vertices[2].x, normalized_vertices[2].y},
                {normalized_vertices[3].x, normalized_vertices[3].y}}),
            ShapeType::RECTANGLE);
      }
      return absl::UnimplementedError(
          "got 4 vertices but no axis aligned rectangle");
    default:
      return absl::UnimplementedError(
          "only points, lines, rectangles and circles are supported");
  }
}

bool HitBox::CollidesWith(const HitBox& other) const {
  switch (other.shape_type_) {
    // Potentially unsafe if not careful.
    case ShapeType::POINT:
      return this->shape_->Collides(*static_cast<Point*>(other.shape_.get()));
    case ShapeType::LINE:
      return this->shape_->Collides(*static_cast<Line*>(other.shape_.get()));
    case ShapeType::RECTANGLE:
      return this->shape_->Collides(
          *static_cast<Rectangle*>(other.shape_.get()));
    case ShapeType::CIRCLE:
      return this->shape_->Collides(*static_cast<Circle*>(other.shape_.get()));
    default:
      // This should never happen.
      // `StatusOr` can be returned here, but it could hurt performance.
      LOG(ERROR) << "HitBox::CollidesWith: invalid shape type";
      return false;
  }
}

void HitBox::Draw() const {
  shape_->Draw();
}

}  // namespace internal
}  // namespace lib
