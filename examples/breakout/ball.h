#ifndef EXAMPLES_BREAKOUT_BALL_H
#define EXAMPLES_BREAKOUT_BALL_H

#include "lib/api/objects/movable_object.h"

namespace breakout {

inline constexpr int BALL = 1;

class Ball : public lib::api::objects::MovableObject {
 public:
  Ball(const MovableObjectOpts options,
       const std::pair<double, double> hit_box_center,
       const double hit_box_radius, double ball_starting_velocity_x,
       double ball_starting_velocity_y)
      : MovableObject(BALL, options, hit_box_center, hit_box_radius),
        ball_starting_velocity_x_(ball_starting_velocity_x),
        ball_starting_velocity_y_(ball_starting_velocity_y),
        has_touched_bottom_(false) {}

  [[nodiscard]] double starting_velocity_x() const {
    return ball_starting_velocity_x_;
  }
  [[nodiscard]] double starting_velocity_y() const {
    return ball_starting_velocity_y_;
  }

  void set_has_touched_bottom() { has_touched_bottom_ = true; }
  [[nodiscard]] bool has_touched_bottom() const { return has_touched_bottom_; }

 protected:
  bool OnCollisionCallback(Object& other_object) override;
  double ball_starting_velocity_x_;
  double ball_starting_velocity_y_;

 private:
  bool has_touched_bottom_;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BALL_H
