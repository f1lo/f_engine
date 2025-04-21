#ifndef LIB_API_OBJECTS_MOVABLE_OBJECT_H
#define LIB_API_OBJECTS_MOVABLE_OBJECT_H

#include <list>
#include <memory>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

class MovableObject : public Object {
 public:
  struct MovableObjectOpts : Opts {
    MovableObjectOpts(bool is_hit_box_active, bool should_draw_hit_box,
                      int velocity_x, int velocity_y)
        : Opts(is_hit_box_active, should_draw_hit_box),
          velocity_x(velocity_x),
          velocity_y(velocity_y) {}
    int velocity_x = 0;
    int velocity_y = 0;
  };

  MovableObject(Kind kind, MovableObjectOpts options,
                const std::vector<std::pair<int, int>>& hit_box_vertices);

  MovableObject(Kind kind, MovableObjectOpts options,
                std::pair<int, int> hit_box_center, uint32_t hit_box_radius);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;
  void Draw() const override;
  void set_velocity(int velocity_x, int velocity_y) {
    velocity_x_ = velocity_x;
    velocity_y_ = velocity_y;
  }

 protected:
  int velocity_x() const { return velocity_x_; }
  int velocity_y() const { return velocity_y_; }

  void Move();
  void ResetLastMove();

 private:
  int velocity_x_;
  int velocity_y_;
  int last_velocity_x_ = 0;
  int last_velocity_y_ = 0;
};
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_MOVABLE_OBJECT_H
