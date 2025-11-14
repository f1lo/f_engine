#ifndef LIB_API_SPRITES_STATIC_SPRITE_H
#define LIB_API_SPRITES_STATIC_SPRITE_H

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

class StaticSprite : public Sprite {
 public:
  ~StaticSprite() override;

  void RotateAndDraw(const WorldPosition draw_destination, const int degree,
                     const int frame_to_draw = 0) const override;
  int total_frames() const override;
  int sprite_height() const override;

 private:
  friend class SpriteFactory;
  friend class SpriteInstance;

  StaticSprite(std::unique_ptr<GraphicsInterface> graphics,
               std::string resource_path);

  const GraphicsInterface* GraphicsForTesting() const override;

  std::unique_ptr<GraphicsInterface> graphics_;
  const Texture2D texture_;
  const Rectangle source_;
  const Vector2 origin_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_STATIC_SPRITE_H
