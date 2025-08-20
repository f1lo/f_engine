#include "raylib/include/raylib.h"

#include "lib/api/abilities/ability.h"

#include <list>
#include <memory>

#include "absl/log/check.h"
#include "lib/api/objects/movable_object.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "lib/api/abilities/keys.h"
#include "lib/api/camera.h"

namespace lib {
namespace api {
namespace abilities {

using api::Camera;
using objects::MovableObject;
using objects::Object;

bool Ability::IsOnCooldown() const {
  return GetTime() - last_used_sec_ <= static_cast<double>(opts_.cooldown_sec);
}

std::list<ObjectAndAbilities> MoveAbility::Use(const Camera& camera) {
  // Generally move should have no cooldown - so ignore it.
  auto* cast_user = dynamic_cast<MovableObject*>(user_);
  CHECK(cast_user) << " ability user is not of correct type.";
  double dir_x = 0;
  double dir_y = 0;
  bool was_used_this_frame = false;
  if (opts_.key_left.has_value() && IsDown(*opts_.key_left)) {
    was_used_this_frame = true;
    dir_x = -1;
  }
  if (opts_.key_right.has_value() && IsDown(*opts_.key_right)) {
    was_used_this_frame = true;
    dir_x = 1;
  }
  if (opts_.key_top.has_value() && IsDown(*opts_.key_top)) {
    was_used_this_frame = true;
    dir_y = -1;
  }
  if (opts_.key_bottom.has_value() && IsDown(*opts_.key_bottom)) {
    was_used_this_frame = true;
    dir_y = 1;
  }

  if (!was_used_this_frame) {
    if (was_used_last_frame_) {
      was_used_last_frame_ = was_used_this_frame;
      // Only zero directions if this is the first frame when movement keys were
      // not hold.
      cast_user->set_direction_global(/*x=*/0, /*y=*/0);
      return {};
    }
    // Do not touch directions.
    was_used_last_frame_ = was_used_this_frame;
    return {};
  }
  was_used_last_frame_ = was_used_this_frame;
  cast_user->set_direction_global(dir_x, dir_y);
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
