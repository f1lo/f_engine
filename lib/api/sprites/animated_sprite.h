#ifndef LIB_API_SPRITES_ANIMATED_SPRITE_H
#define LIB_API_SPRITES_ANIMATED_SPRITE_H

#include "raylib/include/raylib.h"

#include <string>

#include "lib/api/common_types.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

class AnimatedSprite : public Sprite {
 public:
  AnimatedSprite(std::string resource_path, int frame_count);
  ~AnimatedSprite() override;

  void Draw(const WorldPosition draw_destination,
            int frame_to_draw) const override;
  virtual void RotateAndDraw(const WorldPosition draw_destination,
                             const int degree,
                             int frame_to_draw) const override;
  int total_frames() const override;

 private:
  const Texture2D texture_;
  const int frame_count_;
  const float animation_frame_width_;
  const Vector2 origin_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_ANIMATED_SPRITE_H
