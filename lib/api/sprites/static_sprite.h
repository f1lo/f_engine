#ifndef LIB_API_SPRITES_STATIC_SPRITE_H
#define LIB_API_SPRITES_STATIC_SPRITE_H

#include "raylib/include/raylib.h"

#include <string>

#include "lib/api/common_types.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

class SpriteFactory;

class StaticSprite : public Sprite {
 public:
  ~StaticSprite() override;

  void Draw(const WorldPosition draw_destination,
            const int frame_to_draw = 0) const override;
  void RotateAndDraw(const WorldPosition draw_destination, const int degree,
                     const int frame_to_draw = 0) const override;
  int total_frames() const override;
  int sprite_height() const override;

 private:
  friend class SpriteFactory;

  explicit StaticSprite(std::string resource_path);

  const Texture2D texture_;
  const Rectangle source_;
  const Vector2 origin_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_STATIC_SPRITE_H
