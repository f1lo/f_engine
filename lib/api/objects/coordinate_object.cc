#include "raylib/include/raylib.h"

#include "lib/api/objects/coordinate_object.h"

#include <list>
#include <string>

#include "absl/memory/memory.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"

namespace lib {
namespace api {
namespace objects {

std::unique_ptr<CoordinateObject> CoordinateObject::MakeX(
    const float screen_width, const float screen_height) {
  return absl::WrapUnique(new CoordinateObject(
      ScreenPosition{0, kAxisOffset}, ScreenPosition{screen_width, kAxisOffset},
      screen_width, screen_height,
      /*is_x_axis=*/true));
}

std::unique_ptr<CoordinateObject> CoordinateObject::MakeY(
    const float screen_width, const float screen_height) {
  return absl::WrapUnique(new CoordinateObject(
      ScreenPosition{kAxisOffset, 0},
      ScreenPosition{kAxisOffset, screen_height}, screen_width, screen_height,
      /*is_x_axis=*/false));
}

CoordinateObject::CoordinateObject(const ScreenPosition screen_position_start,
                                   const ScreenPosition screen_position_end,
                                   const float screen_width,
                                   const float screen_height,
                                   const bool is_x_axis)
    : Object(ObjectTypeFactory::MakeCoordinate(),
             {/*is_hit_box_active*/ false,
              /*should_draw_hitbox=*/true},
             FLine{screen_position_start.ToFPoint(),
                   screen_position_end.ToFPoint()}),
      is_x_axis_(is_x_axis),
      screen_top_left_pos_(WorldPosition{0, 0}),
      screen_width_(screen_width),
      screen_height_(screen_height) {}

void CoordinateObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {}
bool CoordinateObject::OnCollisionCallback(Object& other_object) {
  return false;
}

void CoordinateObject::DrawX() const {
  float increment = 0;
  while (increment < screen_height_) {
    DrawText(
        std::to_string(static_cast<int>(screen_top_left_pos_.y + increment))
            .c_str(),
        /*posX=*/
        static_cast<int>(screen_top_left_pos_.x - kAxisOffset +
                         kNumbersOffsetY),
        /*posY=*/static_cast<int>(screen_top_left_pos_.y + increment),
        /*fontSize=*/kFontSize,
        /*color=*/BLACK);
    if (increment != 0) {
      DrawLineEx(
          Vector2(screen_top_left_pos_.x, screen_top_left_pos_.y + increment),
          Vector2(screen_top_left_pos_.x + screen_width_,
                  screen_top_left_pos_.y + increment),
          /*thick=*/3.0f, BLACK);
    }

    increment += kDistanceBetweenGrid;
  }
}

void CoordinateObject::DrawY() const {
  float increment = 0;
  while (increment < screen_width_) {
    DrawText(
        std::to_string(static_cast<int>(screen_top_left_pos_.x + increment))
            .c_str(),
        /*posX=*/static_cast<int>(screen_top_left_pos_.x + increment),
        /*posY=*/
        static_cast<int>(screen_top_left_pos_.y - kAxisOffset +
                         kNumbersOffsetX),
        /*fontSize=*/
        kFontSize,
        /*color=*/BLACK);
    if (increment != 0) {
      DrawLineEx(
          Vector2(screen_top_left_pos_.x + increment, screen_top_left_pos_.y),
          Vector2(screen_top_left_pos_.x + increment,
                  screen_top_left_pos_.y + screen_height_),
          /*thick=*/3.0f, BLACK);
    }

    increment += kDistanceBetweenGrid;
  }
}

void CoordinateObject::Draw() const {
  hit_box().Draw();
  if (is_x_axis_) {
    DrawX();
    return;
  }
  DrawY();
}

void CoordinateObject::ReAdjustToScreen(const WorldPosition screen_top_left_pos,
                                        const float screen_width,
                                        const float screen_height) {
  screen_width_ = screen_width;
  screen_height_ = screen_height;
  screen_top_left_pos_ = {screen_top_left_pos.x + kAxisOffset,
                          screen_top_left_pos.y + kAxisOffset};

  if (is_x_axis_) {
    mutable_hit_box().Move(
        screen_width_ / 2 + screen_top_left_pos_.x - center().x,
        screen_top_left_pos_.y - center().y);
    return;
  }

  mutable_hit_box().Move(
      screen_top_left_pos_.x - center().x,
      screen_top_left_pos_.y + screen_height_ / 2 - center().y);
}

}  // namespace objects
}  // namespace api
}  // namespace lib
