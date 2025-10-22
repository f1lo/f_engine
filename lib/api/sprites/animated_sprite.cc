#include "raylib/include/raylib.h"

#include "lib/api/sprites/animated_sprite.h"

#include <string>

#include "lib/api/common_types.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

AnimatedSprite::AnimatedSprite(std::string resource_path, int frame_count)
    : texture_(LoadTexture(resource_path.c_str())),
      frame_count_(frame_count),
      animation_frame_width_((float)texture_.width / (float)frame_count_),
      origin_({animation_frame_width_ / 2, (float)texture_.height / 2}) {}

AnimatedSprite::~AnimatedSprite() {
  UnloadTexture(texture_);
}

void AnimatedSprite::Draw(ScreenPosition draw_destination,
                          int frame_to_draw) const {
  frame_to_draw %= frame_count_;

  DrawTexturePro(texture_,
                 {(float)frame_to_draw * animation_frame_width_, 0.0f,
                  animation_frame_width_, (float)texture_.height},
                 {(float)draw_destination.x, (float)draw_destination.y,
                  animation_frame_width_, (float)texture_.height},
                 origin_, 0.0f, WHITE);
}

void AnimatedSprite::RotateAndDraw(ScreenPosition draw_destination,
                                   const int degree, int frame_to_draw) const {
  frame_to_draw %= frame_count_;

  DrawTexturePro(texture_,
                 {(float)frame_to_draw * animation_frame_width_, 0.0f,
                  animation_frame_width_, (float)texture_.height},
                 {(float)draw_destination.x, (float)draw_destination.y,
                  animation_frame_width_, (float)texture_.height},
                 origin_, degree, WHITE);
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
