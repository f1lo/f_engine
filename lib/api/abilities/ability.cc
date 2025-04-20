#include "lib/api/abilities/ability.h"

#include <optional>

#include "absl/log/check.h"
#include "raylib/include/raylib.h"

namespace lib {
namespace api {
namespace abilities {

using objects::MovableObject;
using objects::Object;

bool Ability::IsOnCooldown() const {
  return GetTime() - last_used_sec_ <= static_cast<double>(opts_.cooldown_sec);
}

void MoveAbility::MaybeUseModifyUser(Object& user) {
  // Generally move should have no cooldown - so ignore it.
  auto* cast_user = dynamic_cast<MovableObject*>(&user);
  CHECK(cast_user) << " ability user is not of correct type.";
  if (opts_.should_hold) {
    int vel_x = 0;
    int vel_y = 0;
    if (IsKeyDown(opts_.key_left)) {
      vel_x -= opts_.velocity_x;
    }
    if (IsKeyDown(opts_.key_right)) {
      vel_x += opts_.velocity_x;
    }
    if (IsKeyDown(opts_.key_top)) {
      vel_y -= opts_.velocity_y;
    }
    if (IsKeyDown(opts_.key_bottom)) {
      vel_y += opts_.velocity_y;
    }
    cast_user->set_velocity(vel_x, vel_y);
    return;
  }

  if (IsKeyPressed(opts_.key_left)) {
    cast_user->set_velocity(-opts_.velocity_x, 0);
  }
  if (IsKeyPressed(opts_.key_right)) {
    cast_user->set_velocity(opts_.velocity_x, 0);
  }
  if (IsKeyPressed(opts_.key_top)) {
    cast_user->set_velocity(0, -opts_.velocity_y);
  }
  if (IsKeyPressed(opts_.key_bottom)) {
    cast_user->set_velocity(0, opts_.velocity_y);
  }
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
