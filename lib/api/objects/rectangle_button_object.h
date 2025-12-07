#ifndef LIB_API_OBJECTS_RECTANGLE_BUTTON_OBJECT_H
#define LIB_API_OBJECTS_RECTANGLE_BUTTON_OBJECT_H

#include <memory>
#include <optional>
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
    ColorRGBA border_color;
    ColorRGBA text_color;
    std::optional<ColorRGBA> fill_color;
  };

  RectangleButtonObject(ObjectType type, WorldPosition lower_left, float width,
                        float height, std::string_view text,
                        const RectangleButtonObjectOpts& options);

  void Draw() const override;

 protected:
 private:
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_RECTANGLE_BUTTON_OBJECT_H
