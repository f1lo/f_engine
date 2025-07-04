#ifndef EXAMPLES_BREAKOUT_LEVEL_MAIN_H
#define EXAMPLES_BREAKOUT_LEVEL_MAIN_H

#include "examples/breakout/ball.h"
#include "lib/api/level.h"

namespace breakout {

class LevelMain final : public lib::api::Level {
 public:
  explicit LevelMain(const lib::api::LevelId id) : Level(id) {}
  ~LevelMain() override = default;

  [[nodiscard]] lib::api::LevelId MaybeChangeLevel() const override;

 private:
  friend class LevelMainBuilder;
  const Ball* ball_ = nullptr;
};

class LevelMainBuilder : public lib::api::LevelBuilder<LevelMain> {
 public:
  explicit LevelMainBuilder(const lib::api::LevelId id, const Ball* ball)
      : LevelBuilder(id) {
    level_->ball_ = ball;
  }
};

}  // namespace breakout

#endif  //EXAMPLES_BREAKOUT_LEVEL_MAIN_H
