#include "examples/breakout/ball.h"

#include "examples/breakout/brick.h"
#include "lib/api/objects/object.h"

namespace breakout {

bool Ball::OnCollisionCallback(Object& other_object) {
  std::pair<float, float> reflected_direction;
  BrickObject* cast_brick;
  if (other_object.type().IsScreenLeft() ||
      other_object.type().IsScreenRight()) {
    SetDirectionGlobal(-direction_x(), direction_y());
    return true;
  }

  if (other_object.type().IsScreenTop()) {
    SetDirectionGlobal(direction_x(), -direction_y());
    return true;
  }

  if (other_object.type().IsScreenBottom()) {
    set_has_touched_bottom();
    return true;
  }

  if (other_object.type().IsPlayer()) {
    reflected_direction =
        other_object.Reflect(*this, direction_x(), direction_y());
    SetDirectionGlobal(reflected_direction.first, reflected_direction.second);
    return true;
  }

  if (other_object.type().IsEnemy()) {
    reflected_direction =
        other_object.Reflect(*this, direction_x(), direction_y());
    SetDirectionGlobal(reflected_direction.first, reflected_direction.second);
    cast_brick = static_cast<BrickObject*>(&other_object);
    cast_brick->set_deleted(true);
    return true;
  }

  return false;
}

}  // namespace breakout
