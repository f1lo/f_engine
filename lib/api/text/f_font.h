#ifndef LIB_API_TEXT_F_FONT
#define LIB_API_TEXT_F_FONT

#include "raylib/include/raylib.h"

#include <memory>

namespace lib {
namespace api {
namespace text {

enum class FontStyle { NORMAL, ITALIC, BOLD };

class FFont {
 private:
  friend class FontFactory;

  Font GetFontStyle(FontStyle style);

  FFont(std::unique_ptr<Font> raylib_font_normal,
        std::unique_ptr<Font> raylib_font_italic,
        std::unique_ptr<Font> raylib_font_bold);

  std::unique_ptr<Font> raylib_font_normal_;
  std::unique_ptr<Font> raylib_font_italic_;
  std::unique_ptr<Font> raylib_font_bold_;
};

}  // namespace text
}  // namespace api
}  // namespace lib

#endif  // LIB_API_TEXT_F_FONT
