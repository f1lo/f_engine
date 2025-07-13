#include "examples/breakout/ball_ability.h"

#include "examples/breakout/ball.h"
#include "lib/api/abilities/keys.h"

namespace breakout {

void BallAbility::MaybeUseModifyUser() {
  if (used_) {
    return;
  }
  if (lib::api::abilities::IsPressed(activation_button_)) {
    used_ = true;
    auto* cast_ball = dynamic_cast<Ball*>(user_);
    CHECK(cast_ball) << " ability user is not of correct type.";
    cast_ball->set_velocity(cast_ball->starting_velocity_x(),
                            cast_ball->starting_velocity_y());
  }
}

}  // namespace breakout
