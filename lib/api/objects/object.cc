#include "lib/api/objects/object.h"

#include <list>

namespace lib {
namespace api {
namespace objects {

bool Object::CollidesWith(const Object& other) const {
  // Hitbox not present or object deleted.
  if (deleted() || !options().is_hit_box_active) {
    return false;
  }
  if (other.deleted() || !other.options().is_hit_box_active) {
    return false;
  }

  return hit_box_.CollidesWith(other.hit_box_);
}

bool Object::UpdateInternal(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  for (const auto& other_object : other_objects) {
    // Skip for the same object.
    if (this == other_object.get()) {
      continue;
    }
    // No collision - skip.
    if (!this->CollidesWith(*other_object.get())) {
      continue;
    }
    // Exit after first collision which changes the object state.
    if (this->OnCollisionCallback(*other_object.get())) {
      return true;
    }
  }

  return false;
}

}  // namespace objects
}  // namespace api
}  // namespace lib