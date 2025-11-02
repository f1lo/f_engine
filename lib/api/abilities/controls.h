#ifndef LIB_API_ABILITIES_CONTROLS_H
#define LIB_API_ABILITIES_CONTROLS_H

#include "raylib/include/raylib.h"

#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace abilities {

typedef int Button;

constexpr Button kKeyD = KEY_D;
constexpr Button kKeyS = KEY_S;
constexpr Button kKeyA = KEY_A;
constexpr Button kKeyW = KEY_W;
constexpr Button kKeySpace = KEY_SPACE;
constexpr Button kKeyEscape = KEY_ESCAPE;

class ControlsInterface {
 public:
  virtual bool IsPressed(const Button button) const = 0;
  virtual bool IsDown(const Button button) const = 0;
  virtual bool IsPrimaryPressed() const = 0;
  virtual bool IsSecondaryPressed() const = 0;
  virtual ScreenPosition GetCursorPos() const = 0;

  virtual ~ControlsInterface() {}
};

class Controls : public ControlsInterface {
 public:
  Controls() = default;

  bool IsPressed(const Button button) const override;
  bool IsDown(const Button button) const override;
  bool IsPrimaryPressed() const override;
  bool IsSecondaryPressed() const override;
  ScreenPosition GetCursorPos() const override;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_CONTROLS_H
