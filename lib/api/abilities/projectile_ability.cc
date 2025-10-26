#include "raylib/include/raylib.h"

#include "lib/api/abilities/projectile_ability.h"

#include <list>
#include <memory>

#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/camera.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/projectile_object.h"

namespace lib {
namespace api {
namespace abilities {

using objects::MovableObject;
using objects::ProjectileObject;

std::list<ObjectAndAbilities> ProjectileAbility::Use(const Camera& camera) {
  if (IsOnCooldown()) {
    return {};
  }
  if (!IsPrimaryPressed() && !IsPressed(kKeySpace)) {
    return {};
  }

  last_used_sec_ = GetTime();

  // Always spawn off user.
  projectile_object_opts_.hit_box_center = {user_->center().x,
                                            user_->center().y};

  // Always ignore the user.
  projectile_object_opts_.ignore_these_objects.insert(user_->kind());

  // If user has direction, spawn projectile with the same direction.
  // Otherwise go top right.
  // TODO(f1lo): If movable object has stopped the directions are zeroed out.
  // This means projectile will not move - probably last known direction
  // needs to be saved to address this.
  double direction_x = 1, direction_y = -1;
  MovableObject* user_movable = dynamic_cast<MovableObject*>(user_);
  if (user_movable != nullptr) {
    direction_x = user_movable->direction_x();
    direction_y = user_movable->direction_y();
  }
  if (direction_x == 0.0f && direction_y == 0.0f) {
    // This is never a good sign. Means projectile is stuck.
    // Can happen if user has not pressed anything yet.
    direction_x = 1.0f;
  }

  std::unique_ptr<ProjectileObject> projectile =
      std::make_unique<ProjectileObject>(
          /*kind=*/projectile_kind_, /*options=*/projectile_object_opts_);
  projectile->SetDirectionGlobal(direction_x, direction_y);

  std::list<ObjectAndAbilities> result;
  result.push_back(ObjectAndAbilities{std::move(projectile),
                                      std::list<std::unique_ptr<Ability>>{}});
  return result;
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
