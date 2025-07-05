#ifndef G_1_PLAYER_H
#define G_1_PLAYER_H

#include "lib/api/objects/movable_object.h"

namespace g_1 {
class Player : public lib::api::objects::MovableObject {
  using MovableObject::MovableObject;

  // TODO(f1lo): Implement.
  bool OnCollisionCallback(Object& other_object) override { return true; }
};
}  // namespace g_1

#endif  // G_1_PLAYER_H
