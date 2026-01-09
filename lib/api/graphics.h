#ifndef LIB_API_GRAPHICS_H
#define LIB_API_GRAPHICS_H

#include "raylib/include/raylib.h"

#include <string>

namespace lib {
namespace api {

class GraphicsInterface {
 public:
  virtual Texture2D Load(std::string resource_path) = 0;
  virtual void Unload(const Texture2D& texture) = 0;
  virtual void Draw(const Texture2D& texture, const Rectangle& source,
                    const Rectangle& dest, const Vector2& origin,
                    float rotation, Color tint) = 0;
  [[nodiscard]] virtual float NativeScreenWidth() const = 0;
  [[nodiscard]] virtual float NativeScreenHeight() const = 0;
  virtual void TextureWrap(Texture2D texture, int wrap) const = 0;

  virtual ~GraphicsInterface() = default;
};

class Graphics : public GraphicsInterface {
 public:
  Graphics(const float native_screen_width, const float native_screen_height)
      : native_screen_width_(native_screen_width),
        native_screen_height_(native_screen_height) {}

  Texture2D Load(std::string resource_path) override;
  void Unload(const Texture2D& texture) override;
  void Draw(const Texture2D& texture, const Rectangle& source,
            const Rectangle& dest, const Vector2& origin, float rotation,
            Color tint) override;
  [[nodiscard]] float NativeScreenWidth() const override;
  [[nodiscard]] float NativeScreenHeight() const override;
  void TextureWrap(Texture2D texture, int wrap) const override;

 private:
  const float native_screen_width_;
  const float native_screen_height_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_GRAPHICS_H