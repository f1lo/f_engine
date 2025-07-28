#ifndef LIB_API_OBJECTS_MOVABLE_OBJECT_H
#define LIB_API_OBJECTS_MOVABLE_OBJECT_H

#include <list>
#include <memory>
#include <optional>
#include <vector>

#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

class MovableObject : public Object {
 public:
  struct MovableObjectOpts : Opts {
    MovableObjectOpts(const bool is_hit_box_active,
                      const bool should_draw_hit_box, const bool attach_camera,
                      const double velocity)
        : Opts(is_hit_box_active, should_draw_hit_box),
          velocity(velocity),
          attach_camera(attach_camera) {}
    double velocity;
    bool attach_camera;
  };

  MovableObject(Kind kind, const MovableObjectOpts& options,
                const std::vector<std::pair<double, double>>& hit_box_vertices);

  MovableObject(Kind kind, const MovableObjectOpts& options,
                std::pair<double, double> hit_box_center,
                double hit_box_radius);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;
  void Draw() const override;

  void set_velocity(const double velocity) { velocity_ = velocity; }
  [[nodiscard]] double velocity() const { return velocity_; }
  void set_direction_global(double x, double y);
  void set_direction_relative(double x, double y);

 protected:
  [[nodiscard]] double direction_x() const { return direction_x_; }
  [[nodiscard]] double direction_y() const { return direction_y_; }

  void Move();
  void ResetLastMove();

 private:
  double velocity_;
  double direction_x_ = 0.0;
  double direction_y_ = 0.0;
  double last_direction_x_ = 0.0;
  double last_direction_y_ = 0.0;
};
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_MOVABLE_OBJECT_H
