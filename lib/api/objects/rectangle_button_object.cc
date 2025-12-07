#include "raylib/include/raylib.h"

#include "lib/api/objects/rectangle_button_object.h"

#include <memory>
#include <optional>
#include <string_view>

#include "lib/api/common_types.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace objects {

RectangleButtonObject::RectangleButtonObject(
    const ObjectType type, const WorldPosition lower_left, const float width,
    const float height, std::string_view text,
    const RectangleButtonObjectOpts& options)
    : StaticObject(type,
                   {.is_hit_box_active = true, .should_draw_hit_box = false},
                   {{}, {}, {}, {}}) {}

void RectangleButtonObject::Draw() const {}

}  // namespace objects
}  // namespace api
}  // namespace lib
