#ifndef LIB_API_TEXT_TEXT_H
#define LIB_API_TEXT_TEXT_H

#include <string>
#include <string_view>

#include "lib/api/common_types.h"
#include "lib/api/text/f_font.h"
#include "lib/internal/shaders/shader_internal.h"
#include "lib/internal/shaders/shader_internal_factory.h"

namespace lib {
namespace api {
namespace text {

class Text {

 public:
  Text(const FFont* f_font, std::string_view text, ColorRGBA color,
       float spacing, float font_size);

  void Draw(FPoint top_left) const;
  void DrawCentered(FPoint center) const;

 private:
  internal::shaders::ShaderInternalFactory& shader_factory_;

  const FFont* f_font_;
  const std::string text_;
  const ColorRGBA color_;
  const float spacing_;
  const float font_size_;

  const internal::shaders::ShaderInternal* shader_;
};

}  // namespace text
}  // namespace api
}  // namespace lib

#endif  // LIB_API_TEXT_TEXT_H
