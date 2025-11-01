#ifndef LIB_API_OBJECTS_STATIC_OBJECT_H
#define LIB_API_OBJECTS_STATIC_OBJECT_H

#include <list>
#include <memory>
#include <optional>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

class StaticObject : public Object {
 public:
  struct StaticObjectOpts : Opts {
    StaticObjectOpts(const bool is_hit_box_active,
                     const bool should_draw_hit_box)
        : Opts(is_hit_box_active, should_draw_hit_box) {}
  };

  StaticObject(Kind kind, StaticObjectOpts options,
               const std::vector<std::pair<double, double>>& hit_box_vertices,
               std::optional<std::unique_ptr<sprites::SpriteInstance>>
                   sprite_instance = std::nullopt);

  StaticObject(Kind kind, StaticObjectOpts options,
               std::pair<double, double> hit_box_center, double hit_box_radius,
               std::optional<std::unique_ptr<sprites::SpriteInstance>>
                   sprite_instance = std::nullopt);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;

  bool OnCollisionCallback(Object& other_object) override;
};
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_STATIC_OBJECT_H
