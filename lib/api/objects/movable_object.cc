#include "lib/api/objects/movable_object.h"

#include <list>
#include <memory>
#include <optional>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_utils.h"

namespace lib {
namespace api {
namespace objects {

MovableObject::MovableObject(
    const Kind kind, const MovableObjectOpts options,
    absl::flat_hash_map<Kind, CollisionCallback> object_collision_callbacks,
    const std::vector<std::pair<int, int>>& hit_box_vertices)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             std::move(object_collision_callbacks),
             CreateHitBoxOrDie(hit_box_vertices)),
      velocity_x_(options.velocity_x),
      velocity_y_(options.velocity_y) {}

MovableObject::MovableObject(
    const Kind kind, const MovableObjectOpts options,
    absl::flat_hash_map<Kind, CollisionCallback> object_collision_callbacks,
    const std::pair<int, int> hit_box_center, const uint32_t hit_box_radius)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             std::move(object_collision_callbacks),
             CreateCircle(hit_box_center.first, hit_box_center.second,
                          hit_box_radius)),
      velocity_x_(options.velocity_x),
      velocity_y_(options.velocity_y) {}

void MovableObject::Move(int velocity_x, int velocity_y) {
  this->mutable_hit_box().Move(velocity_x, velocity_y);
}

void MovableObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  // Try and move first.
  Move(velocity_x_, velocity_y_);
  std::optional<PendingUpdate> pending_update =
      ExecuteUpdates(*this, other_objects);
  // No change to the object, keep initial move and exit.
  if (!pending_update.has_value()) {
    return;
  }
  // Collision happened, revert object back to the original state and update.
  Move(-velocity_x_, -velocity_y_);
  ApplyPendingUpdate(*pending_update);
}

void MovableObject::ApplyPendingUpdate(const PendingUpdate& update) {
  if (update.deleted.has_value()) {
    set_deleted(*update.deleted);
  }
  if (update.is_hit_box_active.has_value()) {
    mutable_options().is_hit_box_active = *update.is_hit_box_active;
  }
  if (update.velocity_x.has_value()) {
    velocity_x_ = *update.velocity_x;
  }
  if (update.velocity_y.has_value()) {
    velocity_y_ = *update.velocity_y;
  }
  if (update.velocity_x_add.has_value()) {
    velocity_x_ += *update.velocity_x_add;
  }
  if (update.velocity_y_add.has_value()) {
    velocity_y_ += *update.velocity_y_add;
  }
  if (update.velocity_x_multiply.has_value()) {
    velocity_x_ *= *update.velocity_x_multiply;
  }
  if (update.velocity_y_multiply.has_value()) {
    velocity_y_ *= *update.velocity_y_multiply;
  }
}

void MovableObject::Draw() const {
  // TODO(f1lo): Draw texture in the future.
  if (!options().should_draw_hit_box) {
    return;
  }
  hit_box().Draw();
}

}  // namespace objects
}  // namespace api
}  // namespace lib
