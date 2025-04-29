#ifndef LIB_API_ABILITIES_KEYS_H
#define LIB_API_ABILITIES_KEYS_H

#include "raylib/include/raylib.h"

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

inline bool IsPressed(const Button button) {
  return IsKeyPressed(button);
}

inline bool IsDown(const Button button) {
  return IsKeyDown(button);
}

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_KEYS_H
