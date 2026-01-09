#ifndef LIB_API_TEXT_F_FONT
#define LIB_API_TEXT_F_FONT

#include "raylib/include/raylib.h"

#include <string_view>

namespace lib {
namespace api {
namespace text {

class FFont {
 public:
  ~FFont();

 private:
  friend class FontFactory;
  friend class Text;

  const Font* GetRaylibFont() const;

  FFont(const std::string_view resource_path);

  const Font raylib_font_;
};

}  // namespace text
}  // namespace api
}  // namespace lib

#endif  // LIB_API_TEXT_F_FONT
