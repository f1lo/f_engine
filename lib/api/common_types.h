#ifndef LIB_API_COMMON_TYPES_H
#define LIB_API_COMMON_TYPES_H

#include <cstdint>
#include <iostream>
#include <optional>

namespace lib {
namespace api {

struct FPoint {
  float x;
  float y;

  bool operator==(const FPoint& other) const;
  friend std::ostream& operator<<(std::ostream& os, const FPoint& point);
};

struct FLine {
  FPoint a;
  FPoint b;

  friend std::ostream& operator<<(std::ostream& os, const FLine& line);
};

struct FRectangle {
  FPoint top_left;
  float width;
  float height;

  friend std::ostream& operator<<(std::ostream& os,
                                  const FRectangle& rectangle);
};

struct FCircle {
  FPoint center;
  float radius;

  friend std::ostream& operator<<(std::ostream& os, const FCircle& circle);
};

class ViewPortContext {
 public:
  ViewPortContext(float screen_width, float screen_height,
                  float native_screen_width, float native_screen_height);

  [[nodiscard]] float scale() const;
  [[nodiscard]] float offset_x() const;
  [[nodiscard]] float offset_y() const;
  [[nodiscard]] float native_screen_width() const;
  [[nodiscard]] float native_screen_height() const;

 private:
  const float scale_;
  const float offset_x_;
  const float offset_y_;
  const float native_screen_width_;
  const float native_screen_height_;
};

struct ScreenPosition {
  float x;
  float y;

  [[nodiscard]] std::optional<ScreenPosition> ToNative(
      const ViewPortContext& ctx) const;
  [[nodiscard]] ScreenPosition ToActual(const ViewPortContext& ctx) const;
  [[nodiscard]] FPoint ToFPoint() const;

  bool operator==(const ScreenPosition& other) const;
  bool operator!=(const ScreenPosition& other) const;
  friend std::ostream& operator<<(std::ostream& os, const ScreenPosition& pos);
};

struct WorldPosition {
  float x;
  float y;

  [[nodiscard]] FPoint ToFPoint() const;

  bool operator==(const WorldPosition& other) const;
  bool operator!=(const WorldPosition& other) const;
  friend std::ostream& operator<<(std::ostream& os, const WorldPosition& pos);
};

struct ColorRGBA {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  static ColorRGBA MakeTransparent();

  friend std::ostream& operator<<(std::ostream& os, const ColorRGBA& color);
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_COMMON_TYPES_H
