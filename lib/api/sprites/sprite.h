#ifndef LIB_API_SPRITES_SPRITE_H
#define LIB_API_SPRITES_SPRITE_H

#include "lib/api/common_types.h"
#include "lib/api/graphics.h"

namespace lib {
namespace api {
namespace sprites {

class Sprite {
 public:
  virtual void RotateAndDraw(WorldPosition draw_destination, int degree,
                             int frame_to_draw = 0) const = 0;
  [[nodiscard]] virtual int total_frames() const = 0;
  [[nodiscard]] virtual int sprite_width() const = 0;
  [[nodiscard]] virtual int sprite_height() const = 0;
  [[nodiscard]] virtual const GraphicsInterface* GraphicsForTesting() const = 0;

  virtual ~Sprite() = default;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_SPRITE_H
