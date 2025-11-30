#include "raylib/include/raylib.h"

#include "lib/api/graphics.h"

#include <string>

namespace lib {
namespace api {

Texture2D Graphics::Load(const std::string resource_path) {
  const Texture2D texture = LoadTexture(resource_path.c_str());
  return texture;
}

void Graphics::Draw(const Texture2D& texture, const Rectangle& source,
                    const Rectangle& dest, const Vector2& origin,
                    const float rotation, const Color tint) {
  DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

void Graphics::Unload(const Texture2D& texture) {
  UnloadTexture(texture);
}

float Graphics::NativeScreenWidth() const {
  return native_screen_width_;
}

float Graphics::NativeScreenHeight() const {
  return native_screen_height_;
}

void Graphics::TextureWrap(Texture2D texture, int wrap) const {
  SetTextureWrap(texture, wrap);
}

}  // namespace api
}  // namespace lib
