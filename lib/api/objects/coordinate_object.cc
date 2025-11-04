#include "raylib/include/raylib.h"

#include "lib/api/objects/coordinate_object.h"

#include <list>
#include <string>

#include "absl/memory/memory.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_utils.h"

namespace lib {
namespace api {
namespace objects {

std::unique_ptr<CoordinateObject> CoordinateObject::MakeX(
    const double screen_width) {
  return absl::WrapUnique(new CoordinateObject(
      ScreenPosition{0, kAxisOffset}, ScreenPosition{screen_width, kAxisOffset},
      /*is_x_axis=*/true));
}
std::unique_ptr<CoordinateObject> CoordinateObject::MakeY(
    const double screen_height) {
  return absl::WrapUnique(
      new CoordinateObject(ScreenPosition{kAxisOffset, 0},
                           ScreenPosition{kAxisOffset, screen_height},
                           /*is_x_axis=*/false));
}

CoordinateObject::CoordinateObject(const ScreenPosition screen_position_start,
                                   const ScreenPosition screen_position_end,
                                   const bool is_x_axis)
    : Object(kCoordinate,
             {/*is_hit_box_active*/ false,
              /*should_draw_hitbox=*/true},
             CreateHitBoxOrDie({screen_position_start, screen_position_end})),
      is_x_axis_(is_x_axis),
      screen_top_left_pos_(WorldPosition{0, 0}) {}

void CoordinateObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {}
bool CoordinateObject::OnCollisionCallback(Object& other_object) {
  return false;
}

void CoordinateObject::Draw() const {
  hit_box().Draw();
  if (is_x_axis_) {
    DrawText(std::to_string((int)screen_top_left_pos_.x).c_str(),
             /*posX=*/screen_top_left_pos_.x,
             /*posY=*/screen_top_left_pos_.y - kAxisOffset + kNumbersOffsetX,
             /*fontSize=*/kFontSize,
             /*color=*/GREEN);
    return;
  }

  DrawText(std::to_string((int)screen_top_left_pos_.y).c_str(),
           /*posX=*/screen_top_left_pos_.x - kAxisOffset + kNumbersOffsetY,
           /*posY=*/screen_top_left_pos_.y, /*fontSize=*/kFontSize,
           /*color=*/GREEN);
}

void CoordinateObject::ReAdjustToScreen(const WorldPosition screen_top_left_pos,
                                        const double screen_width,
                                        const double screen_height) {
  screen_top_left_pos_ = {screen_top_left_pos.x + kAxisOffset,
                          screen_top_left_pos.y + kAxisOffset};

  if (is_x_axis_) {
    mutable_hit_box().Move(
        screen_width / 2 + screen_top_left_pos_.x - center().x,
        screen_top_left_pos_.y - center().y);
    return;
  }

  mutable_hit_box().Move(
      screen_top_left_pos_.x - center().x,
      screen_top_left_pos_.y + screen_height / 2 - center().y);
}

}  // namespace objects
}  // namespace api
}  // namespace lib
