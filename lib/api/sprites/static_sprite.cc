#include "raylib/include/raylib.h"

#include "lib/api/sprites/static_sprite.h"

#include <memory>
#include <string>

#include "lib/api/common_types.h"
#include "lib/api/graphics.h"

namespace lib {
namespace api {
namespace sprites {

StaticSprite::StaticSprite(std::unique_ptr<GraphicsInterface> graphics,
                           const std::string resource_path)
    : graphics_(std::move(graphics)),
      texture_(graphics_->Load(resource_path.c_str())),
      source_({0.0f, 0.0f, (float)texture_.width, (float)texture_.height}),
      origin_(texture_.width / 2, texture_.height / 2) {}

StaticSprite::~StaticSprite() {
  graphics_->Unload(texture_);
}

void StaticSprite::RotateAndDraw(const WorldPosition draw_destination,
                                 const int degree,
                                 const int frame_to_draw) const {
  graphics_->Draw(texture_, source_,
                  {(float)draw_destination.x, (float)draw_destination.y,
                   (float)texture_.width, (float)texture_.height},
                  origin_, degree, WHITE);
}

const GraphicsInterface* StaticSprite::GraphicsForTesting() const {
  return graphics_.get();
}

int StaticSprite::total_frames() const {
  return 1;
}

int StaticSprite::sprite_height() const {
  return texture_.height;
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
