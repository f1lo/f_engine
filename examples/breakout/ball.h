#ifndef EXAMPLES_BREAKOUT_BALL_H
#define EXAMPLES_BREAKOUT_BALL_H

#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object_type.h"

namespace breakout {

class Ball : public lib::api::objects::MovableObject {
 public:
  Ball(const lib::api::objects::ObjectType type,
       const MovableObjectOpts& options,
       const std::pair<float, float> hit_box_center, const float hit_box_radius)
      : MovableObject(type, options, hit_box_center, hit_box_radius),
        ball_starting_velocity_(options.velocity),
        has_touched_bottom_(false) {}

  void set_has_touched_bottom() { has_touched_bottom_ = true; }
  [[nodiscard]] bool has_touched_bottom() const { return has_touched_bottom_; }

 protected:
  bool OnCollisionCallback(Object& other_object) override;
  float ball_starting_velocity_;

 private:
  bool has_touched_bottom_;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BALL_H
