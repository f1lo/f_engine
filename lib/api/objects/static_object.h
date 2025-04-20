#ifndef LIB_API_OBJECTS_STATIC_OBJECT_H
#define LIB_API_OBJECTS_STATIC_OBJECT_H

#include <list>
#include <memory>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

class StaticObject : public Object {
 public:
  struct StaticObjectOpts : Opts {
    StaticObjectOpts(bool is_hit_box_active, bool should_draw_hit_box)
        : Opts(is_hit_box_active, should_draw_hit_box) {}
  };

  StaticObject(Kind kind, StaticObjectOpts options,
               const std::vector<std::pair<int, int>>& hit_box_vertices);

  StaticObject(Kind kind, StaticObjectOpts options,
               std::pair<int, int> hit_box_center, uint32_t hit_box_radius);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;
  void Draw() const override;

 private:
  bool OnCollisionCallback(const Object& other_object) override;
};
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_STATIC_OBJECT_H
