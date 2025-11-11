#ifndef LIB_API_SPRITES_SPRITE_H
#define LIB_API_SPRITES_SPRITE_H

#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace sprites {

class Sprite {
 public:
  virtual void Draw(const WorldPosition draw_destination,
                    const int frame_to_draw = 0) const = 0;
  virtual void RotateAndDraw(const WorldPosition draw_destination,
                             const int degree,
                             const int frame_to_draw = 0) const = 0;
  virtual int total_frames() const = 0;
  virtual int sprite_height() const = 0;

  virtual ~Sprite() {}
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_SPRITE_H
