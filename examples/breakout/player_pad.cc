#include "examples/breakout/player_pad.h"

#include "examples/breakout/ball.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"

namespace breakout {

bool PlayerPad::OnCollisionCallback(Object& other_object) {
  if (other_object.type().IsScreenLeft() ||
      other_object.type().IsScreenRight()) {
    SetDirectionGlobal(-direction_x(), direction_y());
    return true;
  }

  if (other_object.type() == ball_type_) {
    freeze_until_next_set_direction();
    return true;
  }
  return false;
}

}  // namespace breakout
