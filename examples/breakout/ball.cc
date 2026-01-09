#include "examples/breakout/ball.h"

#include "absl/log/check.h"
#include "examples/breakout/brick.h"
#include "lib/api/objects/object.h"

namespace breakout {

bool Ball::OnCollisionCallback(Object& other_object) {
  std::pair<float, float> reflected_direction;
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
    BrickObject* cast_brick = dynamic_cast<BrickObject*>(&other_object);
    CHECK(cast_brick != nullptr);
    cast_brick->set_deleted(true);
    return true;
  }

  return false;
}

}  // namespace breakout
