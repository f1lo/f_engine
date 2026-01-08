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

namespace {

constexpr float kRoundness = 0.2f;
constexpr int kSegments = 10;

}  // namespace

void RectangleButtonObject::DrawRectangleRound() const {
  // Draw fill/inside color first.
  DrawRectangleRounded(raylib_rec_, kRoundness, kSegments, raylib_fill_color_);
  // Draw the border.
  DrawRectangleRoundedLinesEx(raylib_rec_, kRoundness, kSegments,
                              border_thickness_, raylib_border_color_);
}

void RectangleButtonObject::DrawRectangleSharp() const {
  // Draw fill/inside color first.
  DrawRectangle(raylib_rec_.x, raylib_rec_.y, raylib_rec_.width,
                raylib_rec_.height, raylib_fill_color_);
  // Draw the border.
  DrawRectangleLinesEx(raylib_rec_, border_thickness_, raylib_border_color_);
}

RectangleButtonObject::RectangleButtonObject(
    const ObjectType type, const FRectangle& rectangle, std::string_view text,
    const RectangleButtonObjectOpts& options)
    : StaticObject(type,
                   {.is_hit_box_active = true, .should_draw_hit_box = false},
                   rectangle),
      text_(text),
      has_round_corners_(options.has_round_corners),
      border_thickness_(options.border_thickness),
      font_size_(options.font_size),
      spacing_(font_size_ * 0.1f),
      raylib_rec_({.x = rectangle.top_left.x,
                   .y = rectangle.top_left.y,
                   .width = rectangle.width,
                   .height = rectangle.height}),
      raylib_border_color_({.r = options.border_color.r,
                            .g = options.border_color.g,
                            .b = options.border_color.b,
                            .a = options.border_color.a}),
      raylib_text_color_({.r = options.text_color.r,
                          .g = options.text_color.g,
                          .b = options.text_color.b,
                          .a = options.text_color.a}),
      raylib_fill_color_({.r = options.fill_color.r,
                          .g = options.fill_color.g,
                          .b = options.fill_color.b,
                          .a = options.fill_color.a}) {}

void RectangleButtonObject::Draw() const {
  if (has_round_corners_) {
    DrawRectangleRound();
  } else {
    DrawRectangleSharp();
  }
}

}  // namespace objects
}  // namespace api
}  // namespace lib
