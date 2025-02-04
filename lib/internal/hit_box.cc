#include "lib/internal/hit_box.h"

#include <vector>

#include "absl/status/statusor.h"

namespace lib {
namespace internal {

using ::lib::internal::Vertex;

namespace {

bool clockwise(const Vertex &a, const Vertex &b, const Vertex &c) {
  // Avoid overflow.
  return static_cast<long long>(a.x) * (b.y - c.y) +
             static_cast<long long>(b.x) * (c.y - a.y) +
             static_cast<long long>(c.x) * (a.y - b.y) <
         0;
}

bool counter_clockwise(const Vertex &a, const Vertex &b, const Vertex &c) {
  // Avoid overflow.
  return static_cast<long long>(a.x) * (b.y - c.y) +
             static_cast<long long>(b.x) * (c.y - a.y) +
             static_cast<long long>(c.x) * (a.y - b.y) >
         0;
}

/*
 * Constructs convex hull from provided vertices.
 * Returns vertices which are necessary to construct a convex hull.
 */
std::vector<Vertex> construct_convex_hull(std::vector<Vertex> vertices) {
  if (vertices.size() == 1)
    return {vertices[0]};

  std::sort(
      vertices.begin(), vertices.end(),
      [](const Vertex &a, const Vertex &b) { return a.x <= b.x && a.y < b.y; });
  Vertex point_1 = vertices[0];
  Vertex point_2 = vertices.back();
  std::vector<Vertex> up, down;
  up.push_back(point_1);
  down.push_back(point_1);
  for (size_t i = 1; i < vertices.size(); ++i) {
    if (i == vertices.size() - 1 || clockwise(point_1, vertices[i], point_2)) {
      while (up.size() >= 2 &&
             !clockwise(up[up.size() - 2], up[up.size() - 1], vertices[i]))
        up.pop_back();
      up.push_back(vertices[i]);
    }
    if (i == vertices.size() - 1 ||
        counter_clockwise(point_1, vertices[i], point_2)) {
      while (down.size() >= 2 &&
             !counter_clockwise(down[down.size() - 2], down[down.size() - 1],
                                vertices[i]))
        down.pop_back();
      down.push_back(vertices[i]);
    }
  }

  std::vector<Vertex> hull_vertices;
  for (auto i: up)
    hull_vertices.push_back(i);
  for (size_t i = down.size() - 2; i > 0; --i)
    hull_vertices.push_back(down[i]);

  return hull_vertices;
}
} // namespace

absl::StatusOr<HitBox> HitBox::CreateHitBox(std::vector<Vertex> &&vertices) {
  if (vertices.empty()) {
    return absl::InvalidArgumentError(
        "empty vertex set while constructing a hit box");
  }
  const size_t old_size = vertices.size();
  std::vector<Vertex> normalized_vertices =
      construct_convex_hull(std::move(vertices));
  // Some of the vertices have been discarded.
  if (old_size > normalized_vertices.size()) {
    return absl::InvalidArgumentError(
        "hit box construction failed - some of the hit box vertices have been "
        "discarded. This is due to the fact that vertices where provided in "
        "the wrong order or do not form a convex hull");
  }
  if (old_size < normalized_vertices.size()) {
    return absl::InternalError("THIS SHOULD NEVER HAPPEN - vertices added "
                               "while constructing a convex hull.");
  }

  Type type;
  switch (normalized_vertices.size()) {
    case 1:
      type = Type::POINT;
    case 2:
      type = Type::LINE;
    default:
      type = Type::POLYGON;
  }
  return HitBox(std::move(normalized_vertices), type);
}

bool HitBox::Collides(const HitBox &other) {
  /*
   * TODO(f1lo): Implement.
   */
  return true;
}


} // namespace internal
} // namespace lib
