#include "examples/breakout/ball_ability.h"

#include <list>
#include <memory>

#include "examples/breakout/ball.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/camera.h"

namespace breakout {

using lib::api::Camera;
using lib::api::ObjectAndAbilities;

std::list<ObjectAndAbilities> BallAbility::Use(const Camera& camera) {
  if (used_) {
    return {};
  }
  if (lib::api::abilities::IsPressed(activation_button_)) {
    used_ = true;
    auto* cast_ball = dynamic_cast<Ball*>(user_);
    CHECK(cast_ball) << " ability user is not of correct type.";
    cast_ball->set_direction_global(1, -1);
  }

  return {};
}

}  // namespace breakout
