#include "examples/breakout/player_pad.h"

#include "examples/breakout/ball.h"
#include "lib/api/objects/object.h"

namespace breakout {

using lib::api::objects::kScreenLeft;
using lib::api::objects::kScreenRight;

bool PlayerPad::OnCollisionCallback(Object& other_object) {
  if (other_object.kind() == kScreenLeft ||
      other_object.kind() == kScreenRight) {
    SetDirectionGlobal(-direction_x(), direction_y());
    return true;
  }

  if (other_object.kind() == BALL) {
    freeze_until_next_set_direction();
    return true;
  }
  return false;
}

}  // namespace breakout
