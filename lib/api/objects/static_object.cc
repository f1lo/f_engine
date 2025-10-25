#include "lib/api/objects/static_object.h"

#include <list>
#include <memory>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_utils.h"

namespace lib {
namespace api {
namespace objects {

StaticObject::StaticObject(
    const Kind kind, const StaticObjectOpts options,
    const std::vector<std::pair<double, double>>& hit_box_vertices)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateHitBoxOrDie(hit_box_vertices)) {}

StaticObject::StaticObject(const Kind kind, const StaticObjectOpts options,
                           const std::pair<double, double> hit_box_center,
                           const double hit_box_radius)
    : Object(kind,
             {.is_hit_box_active = options.is_hit_box_active,
              .should_draw_hit_box = options.should_draw_hit_box},
             CreateCircle(hit_box_center.first, hit_box_center.second,
                          hit_box_radius)) {}

void StaticObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  UpdateInternal(other_objects);
}

void StaticObject::Draw() const {
  // TODO(f1lo): Draw texture in the future.
  if (!should_draw_hit_box()) {
    return;
  }
  hit_box().Draw();
}

bool StaticObject::OnCollisionCallback(Object& other_object) {
  // TODO(f1lo): Implement.
  return false;
}

}  // namespace objects
}  // namespace api
}  // namespace lib
