#include "lib/api/common_types.h"

#include <algorithm>
#include <cmath>
#include "absl/log/check.h"
#include "absl/strings/substitute.h"

inline constexpr double kEps = 1e-9;

namespace lib {
namespace api {

ViewPortContext::ViewPortContext(const float screen_width,
                                 const float screen_height,
                                 const float native_screen_width,
                                 const float native_screen_height)
    : scale_(std::min(screen_width / native_screen_width,
                      screen_height / native_screen_height)),
      offset_x_((screen_width - native_screen_width * scale_) / 2.0f),
      offset_y_((screen_height - native_screen_height * scale_) / 2.0f),
      native_screen_width_(native_screen_width),
      native_screen_height_(native_screen_height) {
  CHECK(std::abs(scale_) >= kEps)
      << "Bad ViewPortContext, scale is 0."
      << "\nscreen_width: " << screen_width
      << "\nscreen_height: " << screen_height
      << "\native_screen_width: " << native_screen_width
      << "\native_screen_height: " << native_screen_height;
}

float ViewPortContext::scale() const {
  return scale_;
}
float ViewPortContext::offset_x() const {
  return offset_x_;
}
float ViewPortContext::offset_y() const {
  return offset_y_;
}
float ViewPortContext::native_screen_width() const {
  return native_screen_width_;
}
float ViewPortContext::native_screen_height() const {
  return native_screen_height_;
}

std::optional<ScreenPosition> ScreenPosition::ToNative(
    const ViewPortContext& ctx) {
  ScreenPosition native_position = {(x - ctx.offset_x()) / ctx.scale(),
                                    (y - ctx.offset_y()) / ctx.scale()};
  if (native_position.x < 0.0f ||
      native_position.x > ctx.native_screen_width()) {
    return std::nullopt;
  }
  if (native_position.y < 0.0f ||
      native_position.y > ctx.native_screen_height()) {
    return std::nullopt;
  }
  return native_position;
}

ScreenPosition ScreenPosition::ToActual(const ViewPortContext& ctx) {
  return {x * ctx.scale() + ctx.offset_x(), y * ctx.scale() + ctx.offset_y()};
}

bool ScreenPosition::operator==(const ScreenPosition& other) const {
  return std::abs(x - other.x) <= kEps && std::abs(y - other.y) <= kEps;
}

bool ScreenPosition::operator!=(const ScreenPosition& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const ScreenPosition& pos) {
  os << "ScreenPosition (" << pos.x << ", " << pos.y << ")";
  return os;
}

bool WorldPosition::operator==(const WorldPosition& other) const {
  return std::abs(x - other.x) <= kEps && std::abs(y - other.y) <= kEps;
}

bool WorldPosition::operator!=(const WorldPosition& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const WorldPosition& pos) {
  os << "WorldPosition (" << pos.x << ", " << pos.y << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const ColorRGBA& color) {
  os << absl::Substitute("ColorRGBA (r: $0, g: $1, b: $2, a: $3)",
                         static_cast<int>(color.r), static_cast<int>(color.g),
                         static_cast<int>(color.b), static_cast<int>(color.a));
  return os;
}

}  // namespace api
}  // namespace lib
