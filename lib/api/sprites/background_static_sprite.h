#ifndef LIB_API_SPRITES_BACKGROUND_STATIC_SPRITE_H
#define LIB_API_SPRITES_BACKGROUND_STATIC_SPRITE_H

#include "raylib/include/raylib.h"

#include <memory>
#include <string>

#include "lib/api/common_types.h"
#include "lib/api/graphics.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

class BackgroundStaticSprite : public Sprite {
 public:
  ~BackgroundStaticSprite() override;

  void RotateAndDraw(WorldPosition draw_destination, int degree,
                     int frame_to_draw = 0) const override;
  [[nodiscard]] int total_frames() const override;
  [[nodiscard]] int sprite_width() const override;
  [[nodiscard]] int sprite_height() const override;

 protected:
  friend class SpriteFactory;
  friend class SpriteInstance;

  BackgroundStaticSprite(std::unique_ptr<GraphicsInterface> graphics,
                         const std::string& resource_path,
                         float parallax_factor);
  [[nodiscard]] Texture2D texture() const;

 private:
  [[nodiscard]] const GraphicsInterface* GraphicsForTesting() const override;

  std::unique_ptr<GraphicsInterface> graphics_;
  const Texture2D texture_;
  const float screen_width_;
  const float screen_height_;
  const Vector2 origin_;
  const float parallax_factor_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_BACKGROUND_STATIC_SPRITE_H
