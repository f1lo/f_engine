#include "lib/api/abilities/ability.h"

#include <list>
#include <memory>

#include "absl/log/check.h"
#include "lib/api/objects/movable_object.h"
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

std::list<std::unique_ptr<Object>> MoveAbility::Use() {
  // Generally move should have no cooldown - so ignore it.
  // TODO(f1lo): Move the cast in individual statements.
  auto* cast_user = dynamic_cast<MovableObject*>(user_);
  CHECK(cast_user) << " ability user is not of correct type.";
  if (opts_.should_hold) {
    double dir_x = 0;
    double dir_y = 0;
    if (opts_.key_left.has_value() && IsDown(*opts_.key_left)) {
      dir_x = -1;
    }
    if (opts_.key_right.has_value() && IsDown(*opts_.key_right)) {
      dir_x = 1;
    }
    if (opts_.key_top.has_value() && IsDown(*opts_.key_top)) {
      dir_y = -1;
    }
    if (opts_.key_bottom.has_value() && IsDown(*opts_.key_bottom)) {
      dir_y = 1;
    }
    cast_user->set_direction_global(dir_x, dir_y);
    return {};
  }

  double dir_x = 0;
  double dir_y = 0;
  if (opts_.key_left.has_value() && IsPressed(*opts_.key_left)) {
    dir_x = -1;
  }
  if (opts_.key_right.has_value() && IsPressed(*opts_.key_right)) {
    dir_x = 1;
  }
  if (opts_.key_top.has_value() && IsPressed(*opts_.key_top)) {
    dir_y = -1;
  }
  if (opts_.key_bottom.has_value() && IsPressed(*opts_.key_bottom)) {
    dir_y = 1;
  }

  cast_user->set_direction_global(dir_x, dir_y);
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
