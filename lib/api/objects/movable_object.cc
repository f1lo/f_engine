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
    const std::vector<std::pair<double, double>>& hit_box_vertices)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateHitBoxOrDie(hit_box_vertices)),
      velocity_x_(options.velocity_x),
      velocity_y_(options.velocity_y) {}

MovableObject::MovableObject(const Kind kind, const MovableObjectOpts options,
                             const std::pair<double, double> hit_box_center,
                             const double hit_box_radius)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateCircle(hit_box_center.first, hit_box_center.second,
                          hit_box_radius)),
      velocity_x_(options.velocity_x),
      velocity_y_(options.velocity_y) {}

void MovableObject::Move() {
  last_velocity_x_ = velocity_x_;
  last_velocity_y_ = velocity_y_;
  this->mutable_hit_box().Move(velocity_x_, velocity_y_);
}

void MovableObject::ResetLastMove() {
  this->mutable_hit_box().Move(-last_velocity_x_, -last_velocity_y_);
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

}  // namespace objects
}  // namespace api
}  // namespace lib
