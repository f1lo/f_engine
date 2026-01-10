#include "raylib/include/raylib.h"

#include "lib/api/text/text.h"

#include <string_view>

#include "lib/api/common_types.h"
#include "lib/api/text/f_font.h"
#include "lib/internal/shaders/shader_internal_factory.h"

namespace lib {
namespace api {
namespace text {

using internal::shaders::ShaderInternal;
using internal::shaders::ShaderInternalFactory;

Text::Text(const FFont* f_font, const std::string_view text,
           const ColorRGBA color, const float spacing, const float font_size)
    : shader_factory_(ShaderInternalFactory::GetInstance()),
      f_font_(f_font),
      text_(text),
      color_(color),
      spacing_(spacing),
      font_size_(font_size),
      shader_(shader_factory_.MakeFontSDF()) {}

void Text::Draw(const FPoint top_left) const {
  shader_->Activate();
  DrawTextEx(*f_font_->GetRaylibFont(), text_.c_str(),
             {.x = top_left.x, .y = top_left.y}, font_size_, spacing_,
             {.r = color_.r, .g = color_.g, .b = color_.b, .a = color_.a});
  shader_->Deactivate();
}

void Text::DrawCentered(const FPoint center) const {
  const Vector2 width_and_height = MeasureTextEx(
      *f_font_->GetRaylibFont(), text_.c_str(), font_size_, spacing_);

  Draw({.x = center.x - width_and_height.x / 2.0f,
        .y = center.y - width_and_height.y / 2.0f});
}

}  // namespace text
}  // namespace api
}  // namespace lib
