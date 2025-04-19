#include "lib/api/abilities/ability.h"

#include <optional>

#include "raylib/include/raylib.h"

namespace lib {
namespace api {
namespace abilities {

using objects::MovableObject;
using objects::Object;

bool Ability::IsOnCooldown() const {
  return GetTime() - static_cast<double>(last_used_sec_) <=
         static_cast<double>(opts_.cooldown_sec);
}

std::pair<std::optional<Object::PendingUpdate>, std::unique_ptr<MovableObject>>
MoveAbility::MaybeUse() {
  if (!IsKeyPressed(bound_key_) || IsOnCooldown()) {
    return {std::nullopt, nullptr};
  }

  return {Object::PendingUpdate{.deleted = std::nullopt,
                                .is_hit_box_active = std::nullopt,
                                .velocity_x = velocity_x_,
                                .velocity_y = velocity_y_},
          nullptr};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
