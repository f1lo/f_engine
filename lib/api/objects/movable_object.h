#ifndef LIB_API_OBJECTS_MOVABLE_OBJECT_H
#define LIB_API_OBJECTS_MOVABLE_OBJECT_H

#include <list>
#include <memory>
#include <vector>

#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

class MovableObject : public Object {
 public:
  struct MovableObjectOpts {
    bool is_hit_box_active;
    bool should_draw_hit_box;
    bool attach_camera;
    double velocity;
  };

  MovableObject(
      ObjectType type, const MovableObjectOpts& options,
      const std::vector<std::pair<double, double>>& hit_box_vertices,
      std::unique_ptr<sprites::SpriteInstance> sprite_instance = nullptr);

  MovableObject(
      ObjectType type, const MovableObjectOpts& options,
      std::pair<double, double> hit_box_center, double hit_box_radius,
      std::unique_ptr<sprites::SpriteInstance> sprite_instance = nullptr);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;

  void set_velocity(const double velocity) { velocity_ = velocity; }
  void freeze_until_next_set_direction() {
    frozen_until_next_set_direction_ = true;
  }
  [[nodiscard]] double velocity() const { return velocity_; }
  void SetDirectionGlobal(double x, double y);
  void SetDirectionRelative(double x, double y);
  [[nodiscard]] double direction_x() const { return direction_x_; }
  [[nodiscard]] double direction_y() const { return direction_y_; }

 protected:
  virtual void Move();
  virtual void ResetLastMove();

 private:
  [[nodiscard]] bool IsFrozen() const;

  double velocity_;
  double direction_x_ = 0.0;
  double direction_y_ = 0.0;
  double last_direction_x_ = 0.0;
  double last_direction_y_ = 0.0;

  bool frozen_until_next_set_direction_ = false;
};
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_MOVABLE_OBJECT_H
