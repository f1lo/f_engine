#ifndef EXAMPLES_BREAKOUT_PLAYER_PAD_H
#define EXAMPLES_BREAKOUT_PLAYER_PAD_H

#include "lib/api/objects/movable_object.h"

namespace breakout {

constexpr double kPadOffset = 10;
constexpr double kBallOffset = 3;

class PlayerPad final : public lib::api::objects::MovableObject {
 public:
  PlayerPad(const double screen_width, const double screen_height,
            const double player_width, const double player_height,
            const MovableObjectOpts& options)
      : MovableObject(kPlayer, options,
                      {{
                           (screen_width - player_width) / 2.0,
                           screen_height - kPadOffset,
                       },
                       {(screen_width - player_width) / 2.0,
                        screen_height - kPadOffset - player_height},
                       {(screen_width + player_width) / 2.0,
                        screen_height - kPadOffset - player_height},
                       {(screen_width + player_width) / 2.0,
                        screen_height - kPadOffset}}),
        screen_width_(screen_width),
        screen_height_(screen_height),
        player_width_(player_width),
        player_height_(player_height) {}

 protected:
  bool OnCollisionCallback(Object& other_object) override;

 private:
  double screen_width_;
  double screen_height_;
  double player_width_;
  double player_height_;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_PLAYER_PAD_H
