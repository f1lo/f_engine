#include "lib/api/objects/object_utils.h"

#include <list>
#include <memory>
#include <optional>

#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

std::optional<Object::PendingUpdate> ExecuteUpdates(
    const Object& object_to_update,
    const std::list<std::unique_ptr<Object>>& other_objects) {
  for (const auto& other_object : other_objects) {
    // Hitbox not present or object deleted.
    if (object_to_update.deleted() ||
        !object_to_update.options().is_hit_box_active) {
      break;
    }
    if (other_object->deleted() || !other_object->options().is_hit_box_active) {
      continue;
    }
    // No collision - skip.
    if (!object_to_update.hit_box().CollidesWith(other_object->hit_box())) {
      continue;
    }
    // No collision callback provided - skip.
    const auto callback = object_to_update.object_collision_callbacks().find(
        other_object->kind());
    if (callback == object_to_update.object_collision_callbacks().end()) {
      continue;
    }
    // Exit after first collision.
    return callback->second(object_to_update, *other_object.get());
  }

  return std::nullopt;
}

}  // namespace objects
}  // namespace api
}  // namespace lib
