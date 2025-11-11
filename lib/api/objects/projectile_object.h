#ifndef LIB_API_OBJECTS_PROJECTILE_OBJECT_H
#define LIB_API_OBJECTS_PROJECTILE_OBJECT_H

#include "absl/container/flat_hash_set.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

class ProjectileObject : public MovableObject {
 public:
  struct ProjectileObjectOpts {
    bool should_draw_hit_box;
    bool despawn_outside_screen_area;
    double velocity;
    std::pair<double, double> hit_box_center;
    double hit_box_radius;
    absl::flat_hash_set<Kind> despawn_on_colliding_with_these_objects;
    absl::flat_hash_set<Kind> reflect_on_colliding_with_these_objects;
    absl::flat_hash_set<Kind> ignore_these_objects;
  };

  ProjectileObject(
      const Kind kind, ProjectileObjectOpts options,
      std::unique_ptr<sprites::SpriteInstance> sprite_instance = nullptr)
      : MovableObject(kind,
                      MovableObjectOpts{
                          .is_hit_box_active = true,
                          .should_draw_hit_box = options.should_draw_hit_box,
                          .attach_camera = false,
                          .velocity = options.velocity},
                      options.hit_box_center, options.hit_box_radius,
                      std::move(sprite_instance)),
        despawn_outside_screen_area_(options.despawn_outside_screen_area),
        despawn_on_colliding_with_these_objects_(
            std::move(options.despawn_on_colliding_with_these_objects)),
        reflect_on_colliding_with_these_objects_(
            options.reflect_on_colliding_with_these_objects),
        ignore_these_objects_(options.ignore_these_objects) {}

  bool OnCollisionCallback(Object& other_object) override;

 private:
  bool despawn_outside_screen_area_;
  absl::flat_hash_set<Kind> despawn_on_colliding_with_these_objects_;
  absl::flat_hash_set<Kind> reflect_on_colliding_with_these_objects_;
  absl::flat_hash_set<Kind> ignore_these_objects_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  //LIB_API_OBJECTS_PROJECTILE_OBJECT_H
