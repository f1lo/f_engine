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
  virtual int ScreenWidth() const = 0;
  virtual int ScreenHeight() const = 0;
  virtual void TextureWrap(Texture2D texture, int wrap) const = 0;

  virtual ~GraphicsInterface() {}
};

class Graphics : public GraphicsInterface {
 public:
  Graphics() = default;

  Texture2D Load(std::string resource_path) override;
  void Unload(const Texture2D& texture) override;
  void Draw(const Texture2D& texture, const Rectangle& source,
            const Rectangle& dest, const Vector2& origin, float rotation,
            Color tint) override;
  int ScreenWidth() const override;
  int ScreenHeight() const override;
  void TextureWrap(Texture2D texture, int wrap) const override;
};

}  // namespace api
}  // namespace lib

#endif  //LIB_API_GRAPHICS_H