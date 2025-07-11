#ifndef LIB_API_OBJECTS_MOVABLE_OBJECT_H
#define LIB_API_OBJECTS_MOVABLE_OBJECT_H

#include "raylib/include/raylib.h"

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
                      const double velocity_x, const double velocity_y)
        : Opts(is_hit_box_active, should_draw_hit_box),
          attach_camera(attach_camera),
          velocity_x(velocity_x),
          velocity_y(velocity_y) {}
    double velocity_x;
    double velocity_y;
    bool attach_camera;
  };

  MovableObject(Kind kind, const MovableObjectOpts& options,
                const std::vector<std::pair<double, double>>& hit_box_vertices);

  MovableObject(Kind kind, const MovableObjectOpts& options,
                std::pair<double, double> hit_box_center,
                double hit_box_radius);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;
  void Draw() const override;
  void set_velocity(double velocity_x, double velocity_y) {
    velocity_x_ = velocity_x;
    velocity_y_ = velocity_y;
  }

 protected:
  [[nodiscard]] double velocity_x() const { return velocity_x_; }
  [[nodiscard]] double velocity_y() const { return velocity_y_; }

  void Move();
  void ResetLastMove();

 private:
  void BindCamera(double object_center_x, double object_center_y);

  double velocity_x_;
  double velocity_y_;
  double last_velocity_x_ = 0.0;
  double last_velocity_y_ = 0.0;

  std::optional<Camera2D> camera_;
};
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_MOVABLE_OBJECT_H
