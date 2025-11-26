#include "lib/api/objects/projectile_object.h"

#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

bool ProjectileObject::OnCollisionCallback(Object& other_object) {
  if (despawn_outside_screen_area_) {
    if (other_object.type().IsScreenEdge()) {
      set_deleted(true);
      return true;
    }
  }

  if (ignore_these_objects_.contains(other_object.type())) {
    return false;
  }

  if (despawn_on_colliding_with_these_objects_.contains(other_object.type())) {
    set_deleted(true);
    return true;
  }

  if (reflect_on_colliding_with_these_objects_.contains(other_object.type())) {
    auto [dir_x, dir_y] =
        other_object.Reflect(*this, direction_x(), direction_y());

    SetDirectionGlobal(dir_x, dir_y);
    return true;
  }

  return false;
}

}  // namespace objects
}  // namespace api
}  // namespace lib
