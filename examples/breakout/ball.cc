#include "examples/breakout/ball.h"

#include "examples/breakout/brick.h"
#include "lib/api/objects/object.h"

namespace breakout {

using lib::api::objects::kEnemy;
using lib::api::objects::kPlayer;
using lib::api::objects::kScreenBottom;
using lib::api::objects::kScreenLeft;
using lib::api::objects::kScreenRight;
using lib::api::objects::kScreenTop;

bool Ball::OnCollisionCallback(Object& other_object) {
  std::pair<double, double> reflected_direction;
  BrickObject* cast_brick;
  switch (other_object.kind()) {
    case kScreenLeft:
    case kScreenRight:
      SetDirectionGlobal(-direction_x(), direction_y());
      return true;
    case kScreenTop:
      SetDirectionGlobal(direction_x(), -direction_y());
      return true;
    case kScreenBottom:
      set_has_touched_bottom();
      return true;
    case kPlayer:
      reflected_direction =
          other_object.Reflect(*this, direction_x(), direction_y());
      SetDirectionGlobal(reflected_direction.first, reflected_direction.second);
      return true;
    case kEnemy:
      reflected_direction =
          other_object.Reflect(*this, direction_x(), direction_y());
      SetDirectionGlobal(reflected_direction.first, reflected_direction.second);
      // Unsafe.
      cast_brick = static_cast<BrickObject*>(&other_object);
      cast_brick->set_deleted(true);
      return true;
    default:
      return false;
  }
}

}  // namespace breakout
