#include "raylib/include/raylib.h"

#include "lib/api/sprites/background_static_sprite.h"

#include <filesystem>
#include <memory>
#include <string>

#include "lib/api/graphics.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

BackgroundStaticSprite::BackgroundStaticSprite(
    std::unique_ptr<GraphicsInterface> graphics,
    const std::string& resource_path, const float parallax_factor)
    : graphics_(std::move(graphics)),
      texture_(graphics_->Load(
          std::filesystem::path(resource_path).make_preferred().string())),
      screen_width_(graphics_->NativeScreenWidth()),
      screen_height_(graphics_->NativeScreenHeight()),
      origin_(screen_width_ / 2, screen_height_ / 2),
      parallax_factor_(parallax_factor) {
  graphics_->TextureWrap(texture_, TEXTURE_WRAP_REPEAT);
}

BackgroundStaticSprite::~BackgroundStaticSprite() {
  graphics_->Unload(texture_);
}

void BackgroundStaticSprite::RotateAndDraw(const WorldPosition draw_destination,
                                           const int degree,
                                           const int frame_to_draw) const {
  graphics_->Draw(
      texture_,
      {.x = draw_destination.x * parallax_factor_,
       .y = draw_destination.y * parallax_factor_,
       .width = screen_width_,
       .height = screen_height_},
      {draw_destination.x, draw_destination.y, screen_width_, screen_height_},
      origin_, /*rotation=*/0.0, WHITE);
}

const GraphicsInterface* BackgroundStaticSprite::GraphicsForTesting() const {
  return graphics_.get();
}

int BackgroundStaticSprite::total_frames() const {
  return 1;
}

int BackgroundStaticSprite::sprite_width() const {
  return texture_.width;
}

int BackgroundStaticSprite::sprite_height() const {
  return texture_.height;
}

Texture2D BackgroundStaticSprite::texture() const {
  return texture_;
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
