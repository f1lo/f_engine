#ifndef EXAMPLES_BREAKOUT_BALL_ABILITY_H
#define EXAMPLES_BREAKOUT_BALL_ABILITY_H

#include "lib/api/abilities/ability.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "lib/api/abilities/keys.h"
#include "lib/api/objects/object.h"

namespace breakout {

class BallAbility : public lib::api::abilities::Ability {
 public:
  explicit BallAbility(lib::api::abilities::Button activation_button)
      : Ability(/*opts*/ {.cooldown_sec = 0}),
        activation_button_(activation_button) {}

  void MaybeUseModifyUser(lib::api::objects::Object& user) override;

 private:
  lib::api::abilities::Button activation_button_;

  bool used_ = false;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BALL_ABILITY_H
