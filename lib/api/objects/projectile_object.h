#ifndef LIB_API_OBJECTS_PROJECTILE_OBJECT_H
#define LIB_API_OBJECTS_PROJECTILE_OBJECT_H

#include "absl/container/flat_hash_set.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

class ProjectileObject : public MovableObject {
 public:
  struct ProjectileObjectOpts : MovableObjectOpts {
    ProjectileObjectOpts(const bool should_draw_hit_box, const double velocity,
                         const bool despawn_outside_screen_area)
        : MovableObjectOpts(/*is_hit_box_active=*/true, should_draw_hit_box,
                            /*attach_camera=*/false, velocity),
          despawn_outside_screen_area(despawn_outside_screen_area) {}

    bool despawn_outside_screen_area;
  };

  ProjectileObject(
      const Kind kind, const std::pair<double, double> hit_box_center,
      const double hit_box_radius,
      absl::flat_hash_set<Kind> despawn_on_colliding_with_these_objects,
      absl::flat_hash_set<Kind> reflect_on_colliding_with_these_objects,
      absl::flat_hash_set<Kind> ignore_these_objects,
      const ProjectileObjectOpts& options)
      : MovableObject(kind, options, hit_box_center, hit_box_radius),
        despawn_on_colliding_with_these_objects_(
            std::move(despawn_on_colliding_with_these_objects)),
        reflect_on_colliding_with_these_objects_(
            std::move(reflect_on_colliding_with_these_objects)),
        ignore_these_objects_(std::move(ignore_these_objects)),
        opts_(options) {}

  bool OnCollisionCallback(Object& other_object) override;

 private:
  absl::flat_hash_set<Kind> despawn_on_colliding_with_these_objects_;
  absl::flat_hash_set<Kind> reflect_on_colliding_with_these_objects_;
  absl::flat_hash_set<Kind> ignore_these_objects_;
  ProjectileObjectOpts opts_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  //LIB_API_OBJECTS_PROJECTILE_OBJECT_H
