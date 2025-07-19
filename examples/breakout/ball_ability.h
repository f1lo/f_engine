#ifndef EXAMPLES_BREAKOUT_BALL_ABILITY_H
#define EXAMPLES_BREAKOUT_BALL_ABILITY_H

#include <list>
#include <memory>

#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/camera.h"
#include "lib/api/objects/object.h"

namespace breakout {

class BallAbility : public lib::api::abilities::Ability {
 public:
  explicit BallAbility(const lib::api::abilities::Button activation_button)
      : Ability(/*opts*/ {.cooldown_sec = 0}),
        activation_button_(activation_button) {}

  std::list<std::unique_ptr<lib::api::objects::Object>> Use(
      const lib::api::Camera& camera) override;

 private:
  lib::api::abilities::Button activation_button_;

  bool used_ = false;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BALL_ABILITY_H
