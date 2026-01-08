#ifndef LIB_API_OBJECTS_RECTANGLE_BUTTON_OBJECT_H
#define LIB_API_OBJECTS_RECTANGLE_BUTTON_OBJECT_H

#include "raylib/include/raylib.h"

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "lib/api/common_types.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace objects {

class RectangleButtonObject : public StaticObject {
 public:
  struct RectangleButtonObjectOpts {
    bool has_round_corners;
    float border_thickness;
    float font_size;
    ColorRGBA border_color;
    ColorRGBA text_color;
    ColorRGBA fill_color;
  };

  RectangleButtonObject(ObjectType type, const FRectangle& rectangle,
                        std::string_view text,
                        const RectangleButtonObjectOpts& options);

  void Draw() const override;

 private:
  void DrawRectangleRound() const;
  void DrawRectangleSharp() const;

  const std::string text_;
  const bool has_round_corners_;
  const float border_thickness_;
  const float font_size_;
  const float spacing_;
  const Rectangle raylib_rec_;
  const Color raylib_border_color_;
  const Color raylib_text_color_;
  const Color raylib_fill_color_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_RECTANGLE_BUTTON_OBJECT_H
