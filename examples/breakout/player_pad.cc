#include "examples/breakout/player_pad.h"

#include "lib/api/objects/object.h"

namespace breakout {
bool PlayerPad::OnCollisionCallback(const Object& other_object) {
  if (other_object.kind() == SCREEN_LEFT ||
      other_object.kind() == SCREEN_RIGHT) {
    set_velocity(-velocity_x(), velocity_y());
    return true;
  }
  return false;
}

void PlayerPad::Draw() const {
  MovableObject::Draw();
  ball_->Draw();
}

void PlayerPad::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  MovableObject::Update(other_objects);
  ball_->Update(other_objects);
}

}  // namespace breakout
