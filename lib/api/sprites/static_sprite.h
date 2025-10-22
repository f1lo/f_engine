#ifndef LIB_API_SPRITES_STATIC_SPRITE_H
#define LIB_API_SPRITES_STATIC_SPRITE_H

#include "raylib/include/raylib.h"

#include <string>

#include "lib/api/common_types.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

class StaticSprite : public Sprite {
 public:
  StaticSprite(std::string resource_path);
  ~StaticSprite() override;

  void Draw(ScreenPosition draw_destination,
            int frame_to_draw = 0) const override;
  void RotateAndDraw(ScreenPosition draw_destination, const int degree,
                     int frame_to_draw = 0) const override;

 private:
  const Texture2D texture_;
  const Rectangle source_;
  const Vector2 origin_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_STATIC_SPRITE_H
