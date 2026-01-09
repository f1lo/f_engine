#include "raylib/include/raylib.h"

#include "lib/api/text/f_font.h"

#include <memory>

namespace lib {
namespace api {
namespace text {

FFont::FFont(std::unique_ptr<Font> raylib_font_normal,
             std::unique_ptr<Font> raylib_font_italic,
             std::unique_ptr<Font> raylib_font_bold)
    : raylib_font_normal_(std::move(raylib_font_normal)),
      raylib_font_italic_(std::move(raylib_font_italic)),
      raylib_font_bold_(std::move(raylib_font_bold)) {}

Font FFont::GetFontStyle(FontStyle style) {
  switch (style) {
    case FontStyle::NORMAL:
      return *raylib_font_normal_;
    case FontStyle::ITALIC:
      return *raylib_font_italic_;
    case FontStyle::BOLD:
      return *raylib_font_bold_;
  }

  return Font{};
}

}  // namespace text
}  // namespace api
}  // namespace lib
