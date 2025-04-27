#include "examples/breakout/ball_ability.h"

#include "examples/breakout/player_pad.h"
#include "lib/api/abilities/keys.h"

namespace breakout {

void BallAbility::MaybeUseModifyUser(lib::api::objects::Object& user) {
  if (used_) {
    return;
  }
  if (lib::api::abilities::IsPressed(activation_button_)) {
    used_ = true;
    auto* cast_user = dynamic_cast<PlayerPad*>(&user);
    CHECK(cast_user) << " ability user is not of correct type.";
    cast_user->ball().set_velocity(cast_user->ball_velocity_x(),
                                   cast_user->ball_velocity_y());
  }
}

}  // namespace breakout