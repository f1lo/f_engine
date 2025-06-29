#ifndef EXAMPLES_BREAKOUT_LEVEL_MAIN_H
#define EXAMPLES_BREAKOUT_LEVEL_MAIN_H

#include "examples/breakout/ball.h"
#include "lib/api/level.h"

namespace breakout {

class LevelMain final : public lib::api::Level {
 public:
  explicit LevelMain(const lib::api::LevelId id, const Ball* ball)
      : Level(id), ball_(*ball) {};

  ~LevelMain() override = default;

  [[nodiscard]] lib::api::LevelId MaybeChangeLevel() const override;

  const Ball& ball_;
};

}  // namespace breakout

#endif  //EXAMPLES_BREAKOUT_LEVEL_MAIN_H
