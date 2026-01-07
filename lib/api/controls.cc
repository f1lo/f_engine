#include "raylib/include/raylib.h"

#include "lib/api/controls.h"

#include <optional>

#include "lib/api/common_types.h"

namespace lib {
namespace api {

std::optional<WorldPosition> GetMouseWorldPosition(
    const Camera& camera, const ViewPortContext& ctx,
    const ControlsInterface& controls) {
  std::optional<ScreenPosition> native_screen_pos =
      controls.GetCursorPos().ToNative(ctx);
  if (!native_screen_pos.has_value()) {
    return std::nullopt;
  }
  return camera.GetWorldPosition(*native_screen_pos);
}

bool Controls::IsPressed(const Button button) const {
  return IsKeyPressed(button);
}

bool Controls::IsDown(const Button button) const {
  return IsKeyDown(button);
}

bool Controls::IsPrimaryPressed() const {
  return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool Controls::IsSecondaryPressed() const {
  return IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
}

ScreenPosition Controls::GetCursorPos() const {
  return {.x = static_cast<float>(GetMouseX()),
          .y = static_cast<float>(GetMouseY())};
}

}  // namespace api
}  // namespace lib
