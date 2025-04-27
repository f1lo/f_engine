#ifndef EXAMPLES_BREAKOUT_PLAYER_PAD_H
#define EXAMPLES_BREAKOUT_PLAYER_PAD_H

#include <memory>

#include "examples/breakout/ball.h"
#include "lib/api/objects/movable_object.h"

namespace breakout {

constexpr double kPadOffset = 10;
constexpr double kBallOffset = 3;

class PlayerPad final : public lib::api::objects::MovableObject {
 public:
  PlayerPad(double screen_width, double screen_height, double ball_radius,
            double player_width, double player_height, double ball_velocity_x,
            double ball_velocity_y, MovableObjectOpts options)
      : MovableObject(PLAYER, options,
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
        ball_radius_(ball_radius),
        player_width_(player_width),
        player_height_(player_height),
        ball_velocity_x_(ball_velocity_x),
        ball_velocity_y_(ball_velocity_y) {
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
  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;

  [[nodiscard]] Ball& ball() const { return *ball_; }
  [[nodiscard]] double ball_velocity_x() const { return ball_velocity_x_; }
  [[nodiscard]] double ball_velocity_y() const { return ball_velocity_y_; }

 protected:
  bool OnCollisionCallback(const Object& other_object) override;

 private:
  double screen_width_;
  double screen_height_;
  double ball_radius_;
  double player_width_;
  double player_height_;
  double ball_velocity_x_;
  double ball_velocity_y_;
  std::unique_ptr<Ball> ball_;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_PLAYER_PAD_H
