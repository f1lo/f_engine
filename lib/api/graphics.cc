#include "raylib/include/raylib.h"

#include "lib/api/graphics.h"

#include <string>

#include "absl/log/check.h"

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

int Graphics::ScreenWidth() const {
  CHECK(IsWindowReady());
  return GetScreenWidth();
}

int Graphics::ScreenHeight() const {
  CHECK(IsWindowReady());
  return GetScreenHeight();
}

void Graphics::TextureWrap(Texture2D texture, int wrap) const {
  SetTextureWrap(texture, wrap);
}

}  // namespace api
}  // namespace lib
