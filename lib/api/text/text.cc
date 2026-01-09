#include "raylib/include/raylib.h"

#include "lib/api/text/text.h"

#include <string_view>

#include "lib/api/common_types.h"
#include "lib/api/text/f_font.h"

namespace lib {
namespace api {
namespace text {

Text::Text(const FFont* f_font, const std::string_view text,
           const ColorRGBA color, const float spacing, const float font_size)
    : f_font_(f_font),
      text_(text),
      color_(color),
      spacing_(spacing),
      font_size_(font_size) {}

void Text::Draw(const FPoint top_left) const {
  DrawTextEx(*f_font_->GetRaylibFont(), text_.c_str(),
             {.x = top_left.x, .y = top_left.y}, font_size_, spacing_,
             {.r = color_.r, .g = color_.g, .b = color_.b, .a = color_.a});
}

}  // namespace text
}  // namespace api
}  // namespace lib
