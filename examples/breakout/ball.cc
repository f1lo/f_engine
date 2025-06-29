#include "examples/breakout/ball.h"

#include "examples/breakout/brick.h"
#include "lib/api/objects/object.h"

namespace breakout {

bool Ball::OnCollisionCallback(Object& other_object) {
  std::pair<double, double> reflected_velocity;
  BrickObject* cast_brick;
  switch (other_object.kind()) {
    case kScreenLeft:
    case kScreenRight:
      set_velocity(-velocity_x(), velocity_y());
      return true;
    case kScreenTop:
    // TODO(f1lo): Die in this case.
    case kScreenBottom:
      set_has_touched_bottom();
    case kPlayer:
      reflected_velocity =
          other_object.Reflect(*this, velocity_x(), velocity_y());
      set_velocity(reflected_velocity.first, reflected_velocity.second);
      return true;
    case kEnemy:
      reflected_velocity =
          other_object.Reflect(*this, velocity_x(), velocity_y());
      set_velocity(reflected_velocity.first, reflected_velocity.second);
      // Unsafe.
      cast_brick = static_cast<BrickObject*>(&other_object);
      cast_brick->set_deleted(true);
      return true;
    default:
      return false;
  }
}

}  // namespace breakout
