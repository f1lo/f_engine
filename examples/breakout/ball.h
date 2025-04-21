#ifndef EXAMPLES_BREAKOUT_BALL_H
#define EXAMPLES_BREAKOUT_BALL_H

#include "lib/api/objects/movable_object.h"

namespace breakout {

inline constexpr int BALL = 1;

class Ball : public lib::api::objects::MovableObject {
 public:
  Ball() = delete;
  using MovableObject::MovableObject;

 protected:
  bool OnCollisionCallback(const Object& other_object) override;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BALL_H
