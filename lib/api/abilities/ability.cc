#include "raylib/include/raylib.h"

#include "lib/api/abilities/ability.h"

#include <list>
#include <memory>
#include <optional>

#include "absl/log/check.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/controls.h"
#include "lib/api/objects/movable_object.h"

namespace lib {
namespace api {
namespace abilities {

using objects::MovableObject;
using objects::Object;

bool Ability::IsOnCooldown() const {
  return GetTime() - last_used_sec_ <= static_cast<float>(opts_.cooldown_sec);
}

std::list<ObjectAndAbilities> MoveAbility::Use(const AbilityContext& ctx) {
  // Generally move should have no cooldown - so ignore it.
  auto* cast_user = dynamic_cast<MovableObject*>(user());
  CHECK(cast_user) << " ability user is not of correct type.";
  float dir_x = 0;
  float dir_y = 0;
  bool was_used_this_frame = false;
  if (key_left_.has_value() && controls_->IsDown(*key_left_)) {
    was_used_this_frame = true;
    dir_x = -1;
  }
  if (key_right_.has_value() && controls_->IsDown(*key_right_)) {
    was_used_this_frame = true;
    dir_x = 1;
  }
  if (key_top_.has_value() && controls_->IsDown(*key_top_)) {
    was_used_this_frame = true;
    dir_y = -1;
  }
  if (key_bottom_.has_value() && controls_->IsDown(*key_bottom_)) {
    was_used_this_frame = true;
    dir_y = 1;
  }

  if (!was_used_this_frame) {
    if (was_used_last_frame_) {
      was_used_last_frame_ = was_used_this_frame;
      // Freeze object temporarily if this is the first frame when movement
      // keys were not hold.
      cast_user->freeze_until_next_set_direction();
      return {};
    }
    // Do not touch directions.
    was_used_last_frame_ = was_used_this_frame;
    return {};
  }
  was_used_last_frame_ = was_used_this_frame;
  cast_user->SetDirectionGlobal(dir_x, dir_y);
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
