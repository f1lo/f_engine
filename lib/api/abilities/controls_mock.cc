#include "lib/api/abilities/controls_mock.h"

#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace abilities {

ControlsMock::ControlsMock()
    : is_pressed_(true),
      is_down_(true),
      is_primary_pressed_(true),
      is_secondary_pressed_(true),
      cursor_pos_({.x = 0, .y = 0}) {}

ControlsMock::ControlsMock(const bool is_pressed, const bool is_down,
                           const bool is_primary_pressed,
                           const bool is_secondary_pressed,
                           const ScreenPosition cursor_pos)
    : is_pressed_(is_pressed),
      is_down_(is_down),
      is_primary_pressed_(is_primary_pressed),
      is_secondary_pressed_(is_secondary_pressed),
      cursor_pos_(cursor_pos) {}

bool ControlsMock::IsPressed(const Button button) const {
  return is_pressed_;
}

bool ControlsMock::IsDown(const Button button) const {
  return is_down_;
}

bool ControlsMock::IsPrimaryPressed() const {
  return is_primary_pressed_;
}

bool ControlsMock::IsSecondaryPressed() const {
  return is_secondary_pressed_;
}

ScreenPosition ControlsMock::GetCursorPos() const {
  return cursor_pos_;
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
