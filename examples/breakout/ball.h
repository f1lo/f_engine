#ifndef EXAMPLES_BREAKOUT_BALL_H
#define EXAMPLES_BREAKOUT_BALL_H

#include "lib/api/objects/movable_object.h"

namespace breakout {

inline constexpr int BALL = 1;

class Ball : public lib::api::objects::MovableObject {
 public:
  Ball(const Kind kind, const MovableObjectOpts options,
       const std::pair<double, double> hit_box_center,
       const double hit_box_radius, double ball_starting_velocity_x,
       double ball_starting_velocity_y)
      : MovableObject(kind, options, hit_box_center, hit_box_radius),
        ball_starting_velocity_x_(ball_starting_velocity_x),
        ball_starting_velocity_y_(ball_starting_velocity_y) {}

  [[nodiscard]] double starting_velocity_x() const {
    return ball_starting_velocity_x_;
  }
  [[nodiscard]] double starting_velocity_y() const {
    return ball_starting_velocity_y_;
  }

 protected:
  bool OnCollisionCallback(Object& other_object) override;
  double ball_starting_velocity_x_;
  double ball_starting_velocity_y_;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BALL_H
