#ifndef EXAMPLES_BREAKOUT_BALL_ABILITY_H
#define EXAMPLES_BREAKOUT_BALL_ABILITY_H

#include <list>
#include <memory>

#include "lib/api/abilities/ability.h"
#include "lib/api/controls.h"
#include "lib/api/objects/object.h"

namespace breakout {

class BallAbility : public lib::api::abilities::Ability {
 public:
  explicit BallAbility(const lib::api::Button activation_button)
      : Ability(std::make_unique<lib::api::Controls>(),
                /*opts*/ {.cooldown_sec = 0}),
        activation_button_(activation_button) {}

  std::list<lib::api::ObjectAndAbilities> Use(
      const lib::api::abilities::AbilityContext& ctx) override;

 private:
  lib::api::Button activation_button_;

  bool used_ = false;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BALL_ABILITY_H
