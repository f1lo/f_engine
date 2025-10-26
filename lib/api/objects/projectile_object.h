#ifndef LIB_API_OBJECTS_PROJECTILE_OBJECT_H
#define LIB_API_OBJECTS_PROJECTILE_OBJECT_H

#include <optional>

#include "absl/container/flat_hash_set.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

class ProjectileObject : public MovableObject {
 public:
  struct ProjectileObjectOpts : MovableObjectOpts {
    ProjectileObjectOpts(
        const bool should_draw_hit_box, const bool despawn_outside_screen_area,
        const double velocity, const std::pair<double, double> hit_box_center,
        const double hit_box_radius,
        absl::flat_hash_set<Kind> despawn_on_colliding_with_these_objects,
        absl::flat_hash_set<Kind> reflect_on_colliding_with_these_objects,
        absl::flat_hash_set<Kind> ignore_these_objects)
        : MovableObjectOpts(/*is_hit_box_active=*/true, should_draw_hit_box,
                            /*attach_camera=*/false, velocity),
          despawn_outside_screen_area(despawn_outside_screen_area),
          hit_box_center(hit_box_center),
          hit_box_radius(hit_box_radius),
          despawn_on_colliding_with_these_objects(
              std::move(despawn_on_colliding_with_these_objects)),
          reflect_on_colliding_with_these_objects(
              std::move(reflect_on_colliding_with_these_objects)),
          ignore_these_objects(std::move(ignore_these_objects)) {}

    bool despawn_outside_screen_area;
    std::pair<double, double> hit_box_center;
    double hit_box_radius;
    absl::flat_hash_set<Kind> despawn_on_colliding_with_these_objects;
    absl::flat_hash_set<Kind> reflect_on_colliding_with_these_objects;
    absl::flat_hash_set<Kind> ignore_these_objects;
  };

  ProjectileObject(const Kind kind, ProjectileObjectOpts options,
                   std::optional<std::unique_ptr<sprites::SpriteInstance>>
                       sprite_instance = std::nullopt)
      : MovableObject(kind, options, options.hit_box_center,
                      options.hit_box_radius, std::move(sprite_instance)),
        opts_(std::move(options)) {}

  bool OnCollisionCallback(Object& other_object) override;

 private:
  ProjectileObjectOpts opts_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  //LIB_API_OBJECTS_PROJECTILE_OBJECT_H
