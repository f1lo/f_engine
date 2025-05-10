#include "lib/api/abilities/ability.h"

#include <optional>

#include "absl/log/check.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "lib/api/abilities/keys.h"

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
  // TODO(f1lo): Move the cast in individual statements.
  auto* cast_user = dynamic_cast<MovableObject*>(&user);
  CHECK(cast_user) << " ability user is not of correct type.";
  if (opts_.should_hold) {
    double vel_x = 0;
    double vel_y = 0;
    if (IsDown(opts_.key_left)) {
      vel_x = -opts_.velocity_x;
    }
    if (IsDown(opts_.key_right)) {
      vel_x = opts_.velocity_x;
    }
    if (IsDown(opts_.key_top)) {
      vel_y = -opts_.velocity_y;
    }
    if (IsDown(opts_.key_bottom)) {
      vel_y = opts_.velocity_y;
    }
    cast_user->set_velocity(vel_x, vel_y);
    return;
  }

  if (IsPressed(opts_.key_left)) {
    cast_user->set_velocity(-opts_.velocity_x, 0);
  }
  if (IsPressed(opts_.key_right)) {
    cast_user->set_velocity(opts_.velocity_x, 0);
  }
  if (IsPressed(opts_.key_top)) {
    cast_user->set_velocity(0, -opts_.velocity_y);
  }
  if (IsPressed(opts_.key_bottom)) {
    cast_user->set_velocity(0, opts_.velocity_y);
  }
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
