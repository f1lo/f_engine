#include "lib/api/objects/movable_object.h"

#include <list>
#include <memory>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_utils.h"
#include "lib/internal/geometry/vec.h"

namespace lib {
namespace api {
namespace objects {

using internal::Vector;

MovableObject::MovableObject(
    const Kind kind, const MovableObjectOpts& options,
    const std::vector<std::pair<double, double>>& hit_box_vertices)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateHitBoxOrDie(hit_box_vertices)),
      velocity_(options.velocity) {}

MovableObject::MovableObject(const Kind kind, const MovableObjectOpts& options,
                             const std::pair<double, double> hit_box_center,
                             const double hit_box_radius)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateCircle(hit_box_center.first, hit_box_center.second,
                          hit_box_radius)),
      velocity_(options.velocity) {}

void MovableObject::SetDirectionGlobal(const double x, const double y) {
  frozen_until_next_set_direction_ = false;
  const Vector v = Vector{x, y};
  if (v.IsZero()) {
    direction_x_ = 0;
    direction_y_ = 0;
    return;
  }
  const Vector unit = v.ToUnitVector();
  direction_x_ = unit.x;
  direction_y_ = unit.y;
}

void MovableObject::SetDirectionRelative(const double x, const double y) {
  frozen_until_next_set_direction_ = false;
  const WorldPosition world_position = center();
  const Vector v = Vector{x - world_position.x, y - world_position.y};
  if (v.IsZero()) {
    direction_x_ = 0;
    direction_y_ = 0;
    return;
  }
  const Vector unit = v.ToUnitVector();
  direction_x_ = unit.x;
  direction_y_ = unit.y;
}

void MovableObject::Move() {
  if (IsFrozen()) {
    return;
  }
  last_direction_x_ = direction_x_;
  last_direction_y_ = direction_y_;
  this->mutable_hit_box().Move(velocity_ * direction_x_,
                               velocity_ * direction_y_);
}

void MovableObject::ResetLastMove() {
  this->mutable_hit_box().Move(-last_direction_x_ * velocity_,
                               -last_direction_y_ * velocity_);
}

void MovableObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  Move();
  if (UpdateInternal(other_objects)) {
    this->ResetLastMove();
  }
}

void MovableObject::Draw() const {
  // TODO(f1lo): Draw texture in the future.
  if (!options().should_draw_hit_box) {
    return;
  }
  hit_box().Draw();
}

bool MovableObject::IsFrozen() const {
  return frozen_until_next_set_direction_;
}

}  // namespace objects
}  // namespace api
}  // namespace lib
