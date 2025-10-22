#ifndef LIB_API_SPRITES_SPRITE_H
#define LIB_API_SPRITES_SPRITE_H

#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace sprites {

class Sprite {
 public:
  virtual void Draw(ScreenPosition draw_destination,
                    int frame_to_draw = 0) const = 0;
  virtual void RotateAndDraw(ScreenPosition draw_destination, const int degree,
                             int frame_to_draw = 0) const = 0;

  virtual ~Sprite() {}
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_SPRITE_H
