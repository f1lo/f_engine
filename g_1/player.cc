#include "g_1/player.h"

#include "lib/api/objects/object.h"

namespace g_1 {

using lib::api::objects::kProjectilePlayer;

bool Player::OnCollisionCallback(Object& other_object) {
  if (other_object.kind() == kProjectilePlayer) {
    return false;
  }

  return true;
}

}  // namespace g_1
