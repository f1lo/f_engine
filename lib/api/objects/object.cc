#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

bool Object::CollideWith(const Object& other) const {
  // Hitbox not present or object deleted.
  if (deleted() || !options().is_hit_box_active) {
    return false;
  }
  if (other.deleted() || !other.options().is_hit_box_active) {
    return false;
  }

  return hit_box_.CollidesWith(other.hit_box_);
}

}  // namespace objects
}  // namespace api
}  // namespace lib