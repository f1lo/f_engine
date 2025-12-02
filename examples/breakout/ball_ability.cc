#include "examples/breakout/ball_ability.h"

#include <list>
#include <memory>

#include "examples/breakout/ball.h"
#include "lib/api/abilities/controls.h"

namespace breakout {

using lib::api::Camera;
using lib::api::ObjectAndAbilities;
using lib::api::abilities::AbilityContext;

std::list<ObjectAndAbilities> BallAbility::Use(const AbilityContext& ctx) {
  if (used_) {
    return {};
  }
  if (controls_->IsPressed(activation_button_)) {
    used_ = true;
    auto* cast_ball = dynamic_cast<Ball*>(user());
    CHECK(cast_ball) << " ability user is not of correct type.";
    cast_ball->SetDirectionGlobal(1, -1);
  }

  return {};
}

}  // namespace breakout
