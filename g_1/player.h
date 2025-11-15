#ifndef G_1_PLAYER_H
#define G_1_PLAYER_H

#include "lib/api/objects/movable_object.h"

namespace g_1 {

class Player : public lib::api::objects::MovableObject {
  using MovableObject::MovableObject;

  bool OnCollisionCallback(Object& other_object) override;
};

}  // namespace g_1

#endif  // G_1_PLAYER_H
