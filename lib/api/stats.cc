#include "lib/api/stats.h"

#include <optional>

#include "absl/time/time.h"
#include "lib/api/objects/object_type.h"

namespace lib {
namespace api {

void Stats::AddCollision(objects::ObjectType type_1,
                         objects::ObjectType type_2) {
  if (type_2 < type_1) {
    std::swap(type_1, type_2);
  }

  auto [collisions_it_, _] =
      collisions_.try_emplace({type_1, type_2}, CollisionData{});
  collisions_it_->second.collision_count++;
  collisions_it_->second.last_collision_time = absl::Now();
}

std::optional<absl::Time> Stats::GetLastCollisionTime(
    objects::ObjectType type_1, objects::ObjectType type_2) {
  if (type_2 < type_1) {
    std::swap(type_1, type_2);
  }

  const auto collisions_it_ = collisions_.find({type_1, type_2});
  if (collisions_it_ != collisions_.end()) {
    return collisions_it_->second.last_collision_time;
  }

  return std::nullopt;
}

int Stats::GetCollisionCount(objects::ObjectType type_1,
                             objects::ObjectType type_2) {
  if (type_2 < type_1) {
    std::swap(type_1, type_2);
  }
  const auto collisions_it_ = collisions_.find({type_1, type_2});
  if (collisions_it_ != collisions_.end()) {
    return collisions_it_->second.collision_count;
  }

  return 0;
}

}  // namespace api
}  // namespace lib
