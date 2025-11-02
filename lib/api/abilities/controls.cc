#include "raylib/include/raylib.h"

#include "lib/api/abilities/controls.h"

#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace abilities {

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
  return {.x = static_cast<double>(GetMouseX()),
          .y = static_cast<double>(GetMouseY())};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
