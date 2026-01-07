#include "lib/api/objects/static_object.h"

#include <list>
#include <memory>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/object_utils.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

StaticObject::StaticObject(
    const ObjectType type, const StaticObjectOpts options,
    const std::vector<std::pair<float, float>>& hit_box_vertices,
    std::unique_ptr<sprites::SpriteInstance> sprite_instance)
    : Object(type,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateHitBoxOrDie(hit_box_vertices), std::move(sprite_instance)) {}

StaticObject::StaticObject(
    const ObjectType type, const StaticObjectOpts options,
    const std::pair<float, float> hit_box_center, const float hit_box_radius,
    std::unique_ptr<sprites::SpriteInstance> sprite_instance)
    : Object(type,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateCircle(hit_box_center.first, hit_box_center.second,
                          hit_box_radius),
             std::move(sprite_instance)) {}

void StaticObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  UpdateInternal(other_objects);
}

bool StaticObject::OnCollisionCallback(Object& other_object) {
  // TODO(f1lo): Implement.
  return false;
}

}  // namespace objects
}  // namespace api
}  // namespace lib
