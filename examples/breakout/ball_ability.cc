#include "examples/breakout/ball_ability.h"

#include <list>
#include <memory>

#include "examples/breakout/ball.h"
#include "lib/api/abilities/keys.h"

namespace breakout {

std::list<std::unique_ptr<lib::api::objects::Object>> BallAbility::Use() {
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
