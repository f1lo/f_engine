#ifndef LIB_API_SPRITES_ANIMATED_SPRITE_H
#define LIB_API_SPRITES_ANIMATED_SPRITE_H

#include "raylib/include/raylib.h"

#include <memory>
#include <string>

#include "lib/api/common_types.h"
#include "lib/api/graphics.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

class SpriteFactory;
class SpriteInstance;

class AnimatedSprite : public Sprite {
 public:
  ~AnimatedSprite() override;
  virtual void RotateAndDraw(const WorldPosition draw_destination,
                             const int degree,
                             const int frame_to_draw) const override;
  int total_frames() const override;
  int sprite_height() const override;

 private:
  friend class SpriteFactory;
  friend class SpriteInstance;

  AnimatedSprite(std::unique_ptr<GraphicsInterface> graphics,
                 const std::string resource_path, const int frame_count);

  const GraphicsInterface* GraphicsForTesting() const override;

  std::unique_ptr<GraphicsInterface> graphics_;
  const Texture2D texture_;
  const int frame_count_;
  const float animation_frame_width_;
  const Vector2 origin_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_ANIMATED_SPRITE_H
