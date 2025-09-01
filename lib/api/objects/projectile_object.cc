#include "lib/api/objects/projectile_object.h"

#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

bool ProjectileObject::OnCollisionCallback(Object& other_object) {
  if (opts_.despawn_outside_screen_area) {
    if (other_object.kind() == kScreenLeft ||
        other_object.kind() == kScreenRight ||
        other_object.kind() == kScreenTop ||
        other_object.kind() == kScreenBottom) {
      set_deleted(true);
      return true;
    }
  }

  if (opts_.ignore_these_objects.contains(other_object.kind())) {
    return false;
  }

  if (opts_.despawn_on_colliding_with_these_objects.contains(
          other_object.kind())) {
    set_deleted(true);
    return true;
  }

  if (opts_.reflect_on_colliding_with_these_objects.contains(
          other_object.kind())) {
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
