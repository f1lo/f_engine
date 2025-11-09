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
    const double screen_width, const double screen_height) {
  return absl::WrapUnique(new CoordinateObject(
      ScreenPosition{0, kAxisOffset}, ScreenPosition{screen_width, kAxisOffset},
      screen_width, screen_height,
      /*is_x_axis=*/true));
}

std::unique_ptr<CoordinateObject> CoordinateObject::MakeY(
    const double screen_width, const double screen_height) {
  return absl::WrapUnique(new CoordinateObject(
      ScreenPosition{kAxisOffset, 0},
      ScreenPosition{kAxisOffset, screen_height}, screen_width, screen_height,
      /*is_x_axis=*/false));
}

CoordinateObject::CoordinateObject(const ScreenPosition screen_position_start,
                                   const ScreenPosition screen_position_end,
                                   const double screen_width,
                                   const double screen_height,
                                   const bool is_x_axis)
    : Object(kCoordinate,
             {/*is_hit_box_active*/ false,
              /*should_draw_hitbox=*/true},
             CreateHitBoxOrDie({screen_position_start, screen_position_end})),
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
  double increment = 0;
  while (increment < screen_height_) {
    DrawText(
        std::to_string(static_cast<int>(screen_top_left_pos_.y + increment))
            .c_str(),
        /*posX=*/screen_top_left_pos_.x - kAxisOffset + kNumbersOffsetY,
        /*posY=*/screen_top_left_pos_.y + increment,
        /*fontSize=*/kFontSize,
        /*color=*/GREEN);
    if (increment != 0) {
      DrawLineEx(
          Vector2(static_cast<float>(screen_top_left_pos_.x),
                  static_cast<float>(screen_top_left_pos_.y + increment)),
          Vector2(static_cast<float>(screen_top_left_pos_.x + screen_width_),
                  static_cast<float>(screen_top_left_pos_.y + increment)),
          /*thick=*/3.0, BLACK);
    }

    increment += kDistanceBetweenGrid;
  }
}

void CoordinateObject::DrawY() const {
  double increment = 0;
  while (increment < screen_width_) {
    DrawText(
        std::to_string(static_cast<int>(screen_top_left_pos_.x + increment))
            .c_str(),
        /*posX=*/screen_top_left_pos_.x + increment,
        /*posY=*/screen_top_left_pos_.y - kAxisOffset + kNumbersOffsetX,
        /*fontSize=*/kFontSize,
        /*color=*/GREEN);
    if (increment != 0) {
      DrawLineEx(
          Vector2(static_cast<float>(screen_top_left_pos_.x + increment),
                  static_cast<float>(screen_top_left_pos_.y)),
          Vector2(static_cast<float>(screen_top_left_pos_.x + increment),
                  static_cast<float>(screen_top_left_pos_.y + screen_height_)),
          /*thick=*/3.0, BLACK);
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
