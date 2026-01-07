#include "raylib/include/raylib.h"

#include "lib/api/camera.h"

#include "absl/log/check.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {

void Camera::Bind(objects::Object* object) {
  CHECK(!bound_object_) << "Object bound already set.";
  bound_object_ = object;
}

ScreenPosition Camera::GetScreenPosition(const WorldPosition& world_pos) const {
  if (!bound_object_) {
    return {world_pos.x, world_pos.y};
  }
  const Vector2 screen_pos =
      GetWorldToScreen2D({.x = world_pos.x, .y = world_pos.y}, camera_);
  return {screen_pos.x, screen_pos.y};
}

WorldPosition Camera::GetWorldPosition(const ScreenPosition& screen_pos) const {
  if (!bound_object_) {
    return {screen_pos.x, screen_pos.y};
  }

  const Vector2 world_pos =
      GetScreenToWorld2D({.x = screen_pos.x, .y = screen_pos.y}, camera_);
  return {world_pos.x, world_pos.y};
}

void Camera::MaybeActivate() {
  if (!bound_object_) {
    return;
  }

  const WorldPosition world_position = bound_object_->center();
  camera_.target.x = world_position.x;
  camera_.target.y = world_position.y;
  BeginMode2D(camera_);
}

void Camera::MaybeDeactivate() const {
  if (!bound_object_) {
    return;
  }

  EndMode2D();
}

}  // namespace api
}  // namespace lib