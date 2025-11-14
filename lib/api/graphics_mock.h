#ifndef LIB_API_GRAPHICS_MOCK_H
#define LIB_API_GRAPHICS_MOCK_H

#include "raylib/include/raylib.h"

#include <string>

#include "lib/api/graphics.h"

namespace lib {
namespace api {

class GraphicsMock : public GraphicsInterface {
 public:
  GraphicsMock(const unsigned int id, const int texture_width,
               const int texture_height);

  virtual Texture2D Load(const std::string resource_path) override;
  void Unload(const Texture2D& texture) override;
  void Draw(const Texture2D& texture, const Rectangle& source,
            const Rectangle& dest, const Vector2& origin, const float rotation,
            const Color tint) override;

  std::string loaded_texture() const;
  Rectangle drawn_texture_source() const;
  std::string unloaded_texture() const;
  Vector2 drawn_texture_origin() const;
  Texture2D drawn_texture() const;
  unsigned int unloaded_texture_id() const;
  float rotation() const;

 private:
  std::string loaded_texture_;
  Rectangle texture_source_;
  unsigned int unloaded_texture_id_;
  Texture2D texture_to_be_drawn_;
  Vector2 texture_origin_;
  Texture2D drawn_texture_;
  float rotation_;
};

}  // namespace api
}  // namespace lib

#endif  //LIB_API_GRAPHICS_MOCK_H