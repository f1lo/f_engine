#include "raylib/include/raylib.h"

#include "lib/api/graphics_mock.h"

#include "lib/api/graphics.h"

namespace lib {
namespace api {

GraphicsMock::GraphicsMock(const unsigned int id, const int texture_width,
                           const int texture_height)
    : texture_to_be_drawn_({
          .id = id,
          .width = texture_width,
          .height = texture_height,
          .mipmaps = 0,
          .format = 0,
      }) {}

Texture2D GraphicsMock::Load(const std::string resource_path) {
  loaded_texture_ = resource_path;
  return texture_to_be_drawn_;
}

void GraphicsMock::Draw(const Texture2D& texture, const Rectangle& source,
                        const Rectangle& dest, const Vector2& origin,
                        const float rotation, const Color tint) {
  texture_source_ = source;
  texture_origin_ = origin;
  drawn_texture_ = texture;
  rotation_ = rotation;
}

void GraphicsMock::Unload(const Texture2D& texture) {
  unloaded_texture_id_ = texture.id;
}

std::string GraphicsMock::loaded_texture() const {
  return loaded_texture_;
}

Rectangle GraphicsMock::drawn_texture_source() const {
  return texture_source_;
}

unsigned int GraphicsMock::unloaded_texture_id() const {
  return unloaded_texture_id_;
}

Vector2 GraphicsMock::drawn_texture_origin() const {
  return texture_origin_;
}

Texture2D GraphicsMock::drawn_texture() const {
  return drawn_texture_;
}

float GraphicsMock::rotation() const {
  return rotation_;
}

}  // namespace api
}  // namespace lib
