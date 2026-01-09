#include "raylib/include/raylib.h"

#include "lib/api/text/f_font.h"

#include <filesystem>
#include <string_view>

namespace lib {
namespace api {
namespace text {

FFont::FFont(const std::string_view resource_path)
    : raylib_font_(LoadFontEx(std::filesystem::path(resource_path)
                                  .make_preferred()
                                  .string()
                                  .c_str(),
                              128, nullptr, 0)) {}

FFont::~FFont() {
  UnloadFont(raylib_font_);
}

const Font* FFont::GetRaylibFont() const {
  return &raylib_font_;
}

}  // namespace text
}  // namespace api
}  // namespace lib
