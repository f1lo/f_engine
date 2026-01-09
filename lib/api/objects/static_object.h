#ifndef LIB_API_OBJECTS_STATIC_OBJECT_H
#define LIB_API_OBJECTS_STATIC_OBJECT_H

#include <list>
#include <memory>

#include "gtest/gtest_prod.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

class StaticObject : public Object {
 public:
  struct StaticObjectOpts {
    bool is_hit_box_active;
    bool should_draw_hit_box;
  };

  StaticObject(
      ObjectType type, StaticObjectOpts options, const HitBoxVariant& hit_box,
      std::unique_ptr<sprites::SpriteInstance> sprite_instance = nullptr);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;

 protected:
  FRIEND_TEST(StaticObjectTest, OnCollisionCallbackDoesNothing);
  bool OnCollisionCallback(Object& other_object) override;
};
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_STATIC_OBJECT_H
