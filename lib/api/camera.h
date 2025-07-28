#ifndef LIB_API_CAMERA_H
#define LIB_API_CAMERA_H

#include "raylib/include/raylib.h"

#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {

class Camera {
 public:
  Camera()
      : camera_({.offset = {.x = static_cast<float>(GetScreenWidth()) / 2.0f,
                            .y = static_cast<float>(GetScreenHeight()) / 2.0f},
                 .target = {.x = 0.0f, .y = 0.0f},
                 .rotation = 0.0f,
                 .zoom = 1.0f}),
        bound_object_(nullptr) {}

  Camera(const Camera&) = delete;
  Camera& operator=(const Camera&) = delete;
  Camera(const Camera&&) = delete;
  Camera&& operator=(const Camera&&) = delete;

  // Does not take ownership.
  void Bind(objects::Object* object);
  [[nodiscard]] ScreenPosition GetScreenPosition(
      const WorldPosition& world_pos) const;
  [[nodiscard]] WorldPosition GetWorldPosition(
      const ScreenPosition& screen_pos) const;
  void MaybeActivate();
  void MaybeDeactivate() const;

 private:
  Camera2D camera_;
  objects::Object* bound_object_;
};

}  // namespace api
}  // namespace lib
#endif  //LIB_API_CAMERA_H
