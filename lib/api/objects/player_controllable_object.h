#ifndef LIB_API_OBJECTS_PLAYER_CONTROLLABLE_OBJECT_H
#define LIB_API_OBJECTS_PLAYER_CONTROLLABLE_OBJECT_H

#include <list>
#include <memory>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/any_invocable.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

class PlayerControllableObject : public MovableObject {
 public:
  struct PlayerControllableObjectOpts : MovableObjectOpts {
    PlayerControllableObjectOpts(bool is_hit_box_active,
                                 bool should_draw_hit_box, int velocity_x,
                                 int velocity_y)
        : MovableObjectOpts(is_hit_box_active, should_draw_hit_box, velocity_x,
                            velocity_y) {}
  };

  PlayerControllableObject(
      Kind kind, PlayerControllableObjectOpts options,
      const std::vector<std::pair<int, int>>& hit_box_vertices);
  PlayerControllableObject(Kind kind, PlayerControllableObjectOpts options,
                           std::pair<int, int> hit_box_center,
                           uint32_t hit_box_radius);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;
  void Draw() const override;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_PLAYER_CONTROLLABLE_OBJECT_H
