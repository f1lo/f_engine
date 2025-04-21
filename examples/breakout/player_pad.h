#ifndef EXAMPLES_BREAKOUT_PLAYER_PAD_H
#define EXAMPLES_BREAKOUT_PLAYER_PAD_H

#include <memory>

#include "examples/breakout/ball.h"
#include "lib/api/objects/movable_object.h"

namespace breakout {

constexpr int kPadOffset = 10;
constexpr int kBallOffset = 3;

class PlayerPad final : public lib::api::objects::MovableObject {
 public:
  PlayerPad(int screen_width, int screen_height, int ball_speed_x,
            int ball_speed_y, uint32_t ball_radius, int player_width,
            int player_height, MovableObjectOpts options)
      : MovableObject(
            PLAYER, options,
            {{
                 (screen_width - player_width) / 2,
                 screen_height - kPadOffset,
             },
             {(screen_width - player_width) / 2,
              screen_height - kPadOffset - player_height},
             {(screen_width + player_width) / 2,
              screen_height - kPadOffset - player_height},
             {(screen_width + player_width) / 2, screen_height - kPadOffset}}),
        screen_width_(screen_width),
        screen_height_(screen_height),
        ball_speed_x_(ball_speed_x),
        ball_speed_y_(ball_speed_y),
        ball_radius_(ball_radius),
        player_width_(player_width),
        player_height_(player_height) {
    ball_ = std::make_unique<Ball>(
        BALL,
        MovableObjectOpts(/*is_hit_box_active=*/true,
                          /*should_draw_hit_box=*/true, /*velocity_x=*/0,
                          /*velocity_y=*/0),
        /*hit_box_center=*/
        std::make_pair(screen_width / 2, screen_height - kPadOffset -
                                             player_height - kBallOffset -
                                             ball_radius_),
        ball_radius_);
  }

  void Draw() const override;

 protected:
  bool OnCollisionCallback(const Object& other_object) override;

 private:
  int screen_width_;
  int screen_height_;
  int ball_speed_x_;
  int ball_speed_y_;
  uint32_t ball_radius_;
  int player_width_;
  int player_height_;
  std::unique_ptr<Ball> ball_;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_PLAYER_PAD_H
