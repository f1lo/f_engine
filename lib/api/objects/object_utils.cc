#include "lib/api/objects/object_utils.h"

#include <list>
#include <memory>
#include <optional>

#include "absl/log/check.h"
#include "lib/api/objects/object.h"
#include "lib/internal/geometry/shape.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {

using internal::HitBox;
using internal::Point;

std::optional<Object::PendingUpdate> ExecuteUpdates(
    const Object& object_to_update,
    const std::list<std::unique_ptr<Object>>& other_objects) {
  for (const auto& other_object : other_objects) {
    // No collision - skip.
    if (!object_to_update.CollideWith(*other_object.get())) {
      continue;
    }
    // No collision callback provided - skip.
    const auto callback = object_to_update.object_collision_callbacks().find(
        other_object->kind());
    if (callback == object_to_update.object_collision_callbacks().end()) {
      continue;
    }
    // Exit after first collision.
    return callback->second(object_to_update, *other_object.get());
  }

  return std::nullopt;
}

HitBox CreateHitBoxOrDie(const std::vector<std::pair<int, int>>& vertices) {
  // Convert pairs to Points.
  std::vector<Point> points;
  points.reserve(vertices.size());
  for (const auto& [x, y] : vertices) {
    points.emplace_back(x, y);
  }

  absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox(std::move(points));
  CHECK(hit_box.ok()) << "HitBox::CreateHitBox() failed " << hit_box.status();

  return *std::move(hit_box);
}

HitBox CreateCircle(const int x, const int y, const uint32_t radius) {
  return HitBox::CreateHitBox(Point{x, y}, radius);
}

}  // namespace objects
}  // namespace api
}  // namespace lib
