#include "raylib/include/raylib.h"

#include "lib/api/sprites/static_sprite.h"

#include <string>

#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace sprites {

StaticSprite::StaticSprite(std::string resource_path)
    : texture_(LoadTexture(resource_path.c_str())),
      source_({0.0f, 0.0f, (float)texture_.width, (float)texture_.height}),
      origin_(texture_.width / 2, texture_.height / 2) {}

StaticSprite::~StaticSprite() {
  UnloadTexture(texture_);
}

void StaticSprite::Draw(ScreenPosition draw_destination,
                        int frame_to_draw) const {
  DrawTexturePro(texture_, source_,
                 {(float)draw_destination.x, (float)draw_destination.y,
                  (float)texture_.width, (float)texture_.height},
                 origin_, 0.0f, WHITE);
}

void StaticSprite::RotateAndDraw(ScreenPosition draw_destination,
                                 const int degree, int frame_to_draw) const {
  DrawTexturePro(texture_, source_,
                 {(float)draw_destination.x, (float)draw_destination.y,
                  (float)texture_.width, (float)texture_.height},
                 origin_, degree, WHITE);
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
