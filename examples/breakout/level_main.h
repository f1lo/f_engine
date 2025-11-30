#ifndef EXAMPLES_BREAKOUT_LEVEL_MAIN_H
#define EXAMPLES_BREAKOUT_LEVEL_MAIN_H

#include "examples/breakout/ball.h"
#include "lib/api/level.h"

namespace breakout {

class LevelMain final : public lib::api::Level {
 public:
  LevelMain(const lib::api::LevelId id, const float native_screen_width,
            const float native_screen_height)
      : Level(id, native_screen_width, native_screen_height) {}
  ~LevelMain() override = default;

  [[nodiscard]] lib::api::LevelId MaybeChangeLevel() const override;

 private:
  friend class LevelMainBuilder;
  const Ball* ball_ = nullptr;
};

class LevelMainBuilder : public lib::api::LevelBuilder<LevelMain> {
 public:
  LevelMainBuilder(const lib::api::LevelId id, const float native_screen_width,
                   const float native_screen_height, const Ball* ball)
      : LevelBuilder(id, native_screen_width, native_screen_height) {
    level_->ball_ = ball;
  }
};

}  // namespace breakout

#endif  //EXAMPLES_BREAKOUT_LEVEL_MAIN_H
