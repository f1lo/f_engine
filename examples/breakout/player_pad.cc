#include "examples/breakout/player_pad.h"

#include "examples/breakout/ball.h"
#include "lib/api/objects/object.h"

namespace breakout {
bool PlayerPad::OnCollisionCallback(Object& other_object) {
  if (other_object.kind() == kScreenLeft ||
      other_object.kind() == kScreenRight) {
    set_velocity(-velocity_x(), velocity_y());
    return true;
  }

  if (other_object.kind() == BALL) {
    set_velocity(0, 0);
    return true;
  }
  return false;
}

}  // namespace breakout
