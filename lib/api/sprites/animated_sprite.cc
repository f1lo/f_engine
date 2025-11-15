#include "raylib/include/raylib.h"

#include "lib/api/sprites/animated_sprite.h"

#include <memory>
#include <string>

#include "lib/api/common_types.h"
#include "lib/api/graphics.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

AnimatedSprite::AnimatedSprite(std::unique_ptr<GraphicsInterface> graphics,
                               const std::string& resource_path,
                               const int frame_count)
    : graphics_(std::move(graphics)),
      texture_(graphics_->Load(resource_path)),
      frame_count_(frame_count),
      animation_frame_width_(static_cast<float>(texture_.width) /
                             static_cast<float>(frame_count_)),
      origin_({animation_frame_width_ / 2,
               static_cast<float>(texture_.height) / 2}) {}

AnimatedSprite::~AnimatedSprite() {
  graphics_->Unload(texture_);
}

void AnimatedSprite::RotateAndDraw(const WorldPosition draw_destination,
                                   const int degree,
                                   const int frame_to_draw) const {
  graphics_->Draw(
      texture_,
      {static_cast<float>(frame_to_draw % frame_count_) *
           animation_frame_width_,
       0.0f, animation_frame_width_, static_cast<float>(texture_.height)},
      {static_cast<float>(draw_destination.x),
       static_cast<float>(draw_destination.y), animation_frame_width_,
       static_cast<float>(texture_.height)},
      origin_, degree, WHITE);
}

const GraphicsInterface* AnimatedSprite::GraphicsForTesting() const {
  return graphics_.get();
}

int AnimatedSprite::total_frames() const {
  return frame_count_;
}

int AnimatedSprite::sprite_height() const {
  return texture_.height;
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
