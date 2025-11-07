#include "lib/api/objects/projectile_object.h"

#include <optional>

#include "lib/api/objects/object.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

bool ProjectileObject::OnCollisionCallback(Object& other_object) {
  if (despawn_outside_screen_area_) {
    if (other_object.kind() == kScreenLeft ||
        other_object.kind() == kScreenRight ||
        other_object.kind() == kScreenTop ||
        other_object.kind() == kScreenBottom) {
      set_deleted(true);
      return true;
    }
  }

  if (ignore_these_objects_.contains(other_object.kind())) {
    return false;
  }

  if (despawn_on_colliding_with_these_objects_.contains(other_object.kind())) {
    set_deleted(true);
    return true;
  }

  if (reflect_on_colliding_with_these_objects_.contains(other_object.kind())) {
    std::pair<double, double> reflected_direction =
        other_object.Reflect(*this, direction_x(), direction_y());

    SetDirectionGlobal(reflected_direction.first, reflected_direction.second);
    return true;
  }

  return false;
}

}  // namespace objects
}  // namespace api
}  // namespace lib
