#include "examples/breakout/ball.h"

#include "lib/api/objects/object.h"

namespace breakout {

bool Ball::OnCollisionCallback(const Object& other_object) {
  switch (other_object.kind()) {
    case SCREEN_LEFT:
    case SCREEN_RIGHT:
      set_velocity(-velocity_x(), velocity_y());
      return true;
    case SCREEN_TOP:
      set_velocity(velocity_x(), -velocity_y());
      return true;
    case PLAYER:
      set_velocity(velocity_x(), -velocity_y());
    case ENEMY:
      set_velocity(velocity_x(), -velocity_y());
      return true;
    default:
      return false;
  }
}

}  // namespace breakout
