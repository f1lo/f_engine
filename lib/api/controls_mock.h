#ifndef LIB_API_CONTROLS_MOCK_H
#define LIB_API_CONTROLS_MOCK_H

#include "lib/api/controls.h"

#include "lib/api/common_types.h"

namespace lib {
namespace api {

class ControlsMock : public ControlsInterface {
 public:
  ControlsMock();
  ControlsMock(bool is_pressed, bool is_down, bool is_primary_pressed,
               bool is_secondary_pressed, ScreenPosition cursor_pos);

  [[nodiscard]] bool IsPressed(Button button) const override;
  [[nodiscard]] bool IsDown(Button button) const override;
  [[nodiscard]] bool IsPrimaryPressed() const override;
  [[nodiscard]] bool IsSecondaryPressed() const override;
  [[nodiscard]] ScreenPosition GetCursorPos() const override;

 private:
  const bool is_pressed_;
  const bool is_down_;
  const bool is_primary_pressed_;
  const bool is_secondary_pressed_;
  const ScreenPosition cursor_pos_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_CONTROLS_MOCK_H
