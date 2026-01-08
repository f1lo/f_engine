#include "lib/api/objects/screen_edge_object.h"

#include <memory>

#include "absl/memory/memory.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"

namespace lib {
namespace api {
namespace objects {

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeTop(
    const float screen_width, const bool should_draw_hit_box) {
  return absl::WrapUnique(new ScreenEdgeObject(
      ObjectTypeFactory::MakeScreenTop(), should_draw_hit_box,
      ScreenPosition{0, 0}, ScreenPosition{screen_width, 0}));
}

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeBottom(
    const float screen_width, const float screen_height,
    const bool should_draw_hit_box) {
  return absl::WrapUnique(new ScreenEdgeObject(
      ObjectTypeFactory::MakeScreenBottom(), should_draw_hit_box,
      ScreenPosition{0, screen_height},
      ScreenPosition{screen_width, screen_height}));
}

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeLeft(
    const float screen_height, const bool should_draw_hit_box) {
  return absl::WrapUnique(new ScreenEdgeObject(
      ObjectTypeFactory::MakeScreenLeft(), should_draw_hit_box,
      ScreenPosition{0, 0}, ScreenPosition{0, screen_height}));
}

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeRight(
    const float screen_width, const float screen_height,
    const bool should_draw_hit_box) {
  return absl::WrapUnique(
      new ScreenEdgeObject(ObjectTypeFactory::MakeScreenRight(),
                           should_draw_hit_box, ScreenPosition{screen_width, 0},
                           ScreenPosition{screen_width, screen_height}));
}

ScreenEdgeObject::ScreenEdgeObject(const ObjectType type,
                                   const bool should_draw_hit_box,
                                   ScreenPosition a, ScreenPosition b)
    : Object(type,
             {/*is_hit_box_active*/ true,
              /*should_draw_hitbox=*/should_draw_hit_box},
             FLine({a.ToFPoint(), b.ToFPoint()})) {}

void ScreenEdgeObject::ReAdjustToScreen(const WorldPosition screen_top_left_pos,
                                        const float screen_width,
                                        const float screen_height) {
  if (type().IsScreenTop()) {
    mutable_hit_box().Move(
        screen_width / 2 + screen_top_left_pos.x - center().x,
        screen_top_left_pos.y - center().y);
    return;
  }
  if (type().IsScreenBottom()) {
    mutable_hit_box().Move(
        screen_width / 2 + screen_top_left_pos.x - center().x,
        screen_height + screen_top_left_pos.y - center().y);
    return;
  }
  if (type().IsScreenLeft()) {
    mutable_hit_box().Move(
        screen_top_left_pos.x - center().x,
        screen_top_left_pos.y + screen_height / 2 - center().y);
    return;
  }
  if (type().IsScreenRight()) {
    mutable_hit_box().Move(
        screen_width + screen_top_left_pos.x - center().x,
        screen_top_left_pos.y + screen_height / 2 - center().y);
    return;
  }
}

}  // namespace objects
}  // namespace api
}  // namespace lib
