#ifndef LIB_API_GRAPHICS_H
#define LIB_API_GRAPHICS_H

#include "raylib/include/raylib.h"

#include <string>

namespace lib {
namespace api {

class GraphicsInterface {
 public:
  virtual Texture2D Load(const std::string resource_path) = 0;
  virtual void Unload(const Texture2D& texture) = 0;
  virtual void Draw(const Texture2D& texture, const Rectangle& source,
                    const Rectangle& dest, const Vector2& origin,
                    const float rotation, const Color tint) = 0;

  virtual ~GraphicsInterface() {}
};

class Graphics : public GraphicsInterface {
 public:
  Graphics() = default;

  virtual Texture2D Load(const std::string resource_path) override;
  void Unload(const Texture2D& texture) override;
  void Draw(const Texture2D& texture, const Rectangle& source,
            const Rectangle& dest, const Vector2& origin, const float rotation,
            const Color tint) override;
};

}  // namespace api
}  // namespace lib

#endif  //LIB_API_GRAPHICS_H