#include <vector>

#include "absl/log/check.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object_utils.h"
#include "lib/internal/geometry/shape.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {

using internal::HitBox;
using internal::PointInternal;

namespace {

template <typename T>
HitBox CreateHitBoxOrDieInternal(const std::vector<T>& vertices) {
  std::vector<PointInternal> points;
  points.reserve(vertices.size());
  for (const auto& [x, y] : vertices) {
    points.emplace_back(x, y);
  }

  absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox(std::move(points));
  CHECK(hit_box.ok()) << "HitBox::CreateHitBox() failed " << hit_box.status();

  return *std::move(hit_box);
}

}  // namespace

HitBox CreateHitBoxOrDie(
    const std::vector<std::pair<double, double>>& vertices) {
  return CreateHitBoxOrDieInternal<std::pair<double, double>>(vertices);
}

HitBox CreateHitBoxOrDie(const std::vector<ScreenPosition>& vertices) {
  return CreateHitBoxOrDieInternal<ScreenPosition>(vertices);
}

HitBox CreateCircle(const double x, const double y, const double radius) {
  return HitBox::CreateHitBox(PointInternal{x, y}, radius);
}

}  // namespace objects
}  // namespace api
}  // namespace lib
