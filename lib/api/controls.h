#ifndef LIB_API_CONTROLS_H
#define LIB_API_CONTROLS_H

#include "raylib/include/raylib.h"

#include "lib/api/common_types.h"

namespace lib {
namespace api {

typedef int Button;

constexpr Button kKeyD = KEY_D;
constexpr Button kKeyS = KEY_S;
constexpr Button kKeyA = KEY_A;
constexpr Button kKeyW = KEY_W;
constexpr Button kKeySpace = KEY_SPACE;
constexpr Button kKeyEscape = KEY_ESCAPE;

class ControlsInterface {
 public:
  [[nodiscard]] virtual bool IsPressed(Button button) const = 0;
  [[nodiscard]] virtual bool IsDown(Button button) const = 0;
  [[nodiscard]] virtual bool IsPrimaryPressed() const = 0;
  [[nodiscard]] virtual bool IsSecondaryPressed() const = 0;
  [[nodiscard]] virtual ScreenPosition GetCursorPos() const = 0;

  virtual ~ControlsInterface() = default;
};

class Controls : public ControlsInterface {
 public:
  Controls() = default;

  [[nodiscard]] bool IsPressed(Button button) const override;
  [[nodiscard]] bool IsDown(Button button) const override;
  [[nodiscard]] bool IsPrimaryPressed() const override;
  [[nodiscard]] bool IsSecondaryPressed() const override;
  [[nodiscard]] ScreenPosition GetCursorPos() const override;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_CONTROLS_H
