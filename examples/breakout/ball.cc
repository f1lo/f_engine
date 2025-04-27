#include "examples/breakout/ball.h"

#include "lib/api/objects/object.h"

namespace breakout {

bool Ball::OnCollisionCallback(const Object& other_object) {
  std::pair<double, double> reflected_velocity;
  switch (other_object.kind()) {
    case SCREEN_LEFT:
    case SCREEN_RIGHT:
      set_velocity(-velocity_x(), velocity_y());
      return true;
    case SCREEN_TOP:
    case PLAYER:
      set_velocity(velocity_x(), -velocity_y());
      set_velocity(velocity_x(), -velocity_y());
      return true;
    case ENEMY:
      reflected_velocity = Reflect(other_object, velocity_x(), velocity_y());
      set_velocity(reflected_velocity.first, reflected_velocity.second);
      return true;
    default:
      return false;
  }
}

}  // namespace breakout
