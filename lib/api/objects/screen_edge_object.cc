#include "lib/api/objects/screen_edge_object.h"

#include <memory>

#include "absl/log/log.h"
#include "absl/memory/memory.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_utils.h"

namespace lib {
namespace api {
namespace objects {

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeTop(
    const double screen_width, const bool should_draw_hit_box) {
  return absl::WrapUnique(new ScreenEdgeObject(
      kScreenTop, should_draw_hit_box, ScreenPosition{0, 0},
      ScreenPosition{screen_width, 0}));
}

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeBottom(
    const double screen_width, const double screen_height,
    const bool should_draw_hit_box) {
  return absl::WrapUnique(new ScreenEdgeObject(
      kScreenBottom, should_draw_hit_box, ScreenPosition{0, screen_height},
      ScreenPosition{screen_width, screen_height}));
}

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeLeft(
    const double screen_height, const bool should_draw_hit_box) {
  return absl::WrapUnique(new ScreenEdgeObject(
      kScreenLeft, should_draw_hit_box, ScreenPosition{0, 0},
      ScreenPosition{0, screen_height}));
}

std::unique_ptr<ScreenEdgeObject> ScreenEdgeObject::MakeRight(
    const double screen_width, const double screen_height,
    const bool should_draw_hit_box) {
  return absl::WrapUnique(new ScreenEdgeObject(
      kScreenRight, should_draw_hit_box, ScreenPosition{screen_width, 0},
      ScreenPosition{screen_width, screen_height}));
}

ScreenEdgeObject::ScreenEdgeObject(const Kind kind,
                                   const bool should_draw_hit_box,
                                   ScreenPosition a, ScreenPosition b)
    : Object(kind,
             {/*is_hit_box_active*/ true,
              /*should_draw_hitbox=*/should_draw_hit_box},
             CreateHitBoxOrDie({a, b})) {}

void ScreenEdgeObject::ReAdjustToScreen(const WorldPosition screen_top_left_pos,
                                        const double screen_width,
                                        const double screen_height) {
  switch (kind()) {
    case kScreenTop:
      mutable_hit_box().Move(
          screen_width / 2 + screen_top_left_pos.x - center().x,
          screen_top_left_pos.y - center().y);
      break;
    case kScreenBottom:
      mutable_hit_box().Move(
          screen_width / 2 + screen_top_left_pos.x - center().x,
          screen_height + screen_top_left_pos.y - center().y);
      break;
    case kScreenLeft:
      mutable_hit_box().Move(
          screen_top_left_pos.x - center().x,
          screen_top_left_pos.y + screen_height / 2 - center().y);
      break;
    case kScreenRight:
      mutable_hit_box().Move(
          screen_width + screen_top_left_pos.x - center().x,
          screen_top_left_pos.y + screen_height / 2 - center().y);
      break;
    default:
      LOG(FATAL) << "Unrecognized screen edge kind.";
  }
}

}  // namespace objects
}  // namespace api
}  // namespace lib
