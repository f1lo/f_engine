#include "lib/api/objects/static_object.h"

#include <list>
#include <memory>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_utils.h"

namespace lib {
namespace api {
namespace objects {

StaticObject::StaticObject(
    const Kind kind, const StaticObjectOpts options,
    absl::flat_hash_map<Kind, CollisionCallback> object_collision_callbacks,
    const std::vector<std::pair<int, int>>& hit_box_vertices)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             std::move(object_collision_callbacks),
             CreateHitBoxOrDie(hit_box_vertices)) {}

StaticObject::StaticObject(
    const Kind kind, const StaticObjectOpts options,
    absl::flat_hash_map<Kind, CollisionCallback> object_collision_callbacks,
    const std::pair<int, int> hit_box_center, const uint32_t hit_box_radius)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             std::move(object_collision_callbacks),
             CreateCircle(hit_box_center.first, hit_box_center.second,
                          hit_box_radius)) {}

void StaticObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  std::optional<PendingUpdate> pending_update =
      ExecuteUpdates(*this, other_objects);
  // No change to the object, exit.
  if (!pending_update.has_value()) {
    return;
  }
  ApplyPendingUpdate(*pending_update);
}

void StaticObject::Draw() const {
  // TODO(f1lo): Draw texture in the future.
  if (!options().should_draw_hit_box) {
    return;
  }
  hit_box().Draw();
}

void StaticObject::ApplyPendingUpdate(const PendingUpdate& update) {
  if (update.deleted.has_value()) {
    set_deleted(*update.deleted);
  }
  if (update.is_hit_box_active.has_value()) {
    mutable_options().is_hit_box_active = *update.is_hit_box_active;
  }
}

}  // namespace objects
}  // namespace api
}  // namespace lib
