#ifndef LIB_API_ABILITIES_CONTROLS_MOCK_H
#define LIB_API_ABILITIES_CONTROLS_MOCK_H

#include "lib/api/abilities/controls.h"

#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace abilities {

class ControlsMock : public ControlsInterface {
 public:
  ControlsMock();
  ControlsMock(const bool is_pressed, const bool is_down,
               const bool is_primary_pressed, const bool is_secondary_pressed,
               const ScreenPosition cursor_pos);

  bool IsPressed(const Button button) const override;
  bool IsDown(const Button button) const override;
  bool IsPrimaryPressed() const override;
  bool IsSecondaryPressed() const override;
  ScreenPosition GetCursorPos() const override;

 private:
  const bool is_pressed_;
  const bool is_down_;
  const bool is_primary_pressed_;
  const bool is_secondary_pressed_;
  const ScreenPosition cursor_pos_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_CONTROLS_MOCK_H
