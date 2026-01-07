#ifndef LIB_INTERNAL_GEOMETRY_SHAPE_H
#define LIB_INTERNAL_GEOMETRY_SHAPE_H

#include <cmath>
#include <memory>
#include <utility>

#include "absl/log/check.h"
#include "absl/strings/substitute.h"
#include "vec.h"

namespace lib {
namespace internal {

struct PointInternal;
struct LineInternal;
struct RectangleInternal;
struct CircleInternal;

struct Shape {
  [[nodiscard]] virtual bool Collides(const PointInternal& point) const = 0;
  [[nodiscard]] virtual bool Collides(const LineInternal& line) const = 0;
  [[nodiscard]] virtual bool Collides(
      const RectangleInternal& rectangle) const = 0;
  [[nodiscard]] virtual bool Collides(const CircleInternal& circle) const = 0;
  virtual void Draw() const = 0;
  virtual void Move(float x, float y) = 0;

  [[nodiscard]] virtual float center_x() const = 0;
  [[nodiscard]] virtual float center_y() const = 0;

  virtual ~Shape() = default;
};

struct PointInternal final : Shape {
  [[nodiscard]] bool Collides(const PointInternal& other_point) const override;
  [[nodiscard]] bool Collides(const LineInternal& line) const override;
  [[nodiscard]] bool Collides(
      const RectangleInternal& rectangle) const override;
  [[nodiscard]] bool Collides(const CircleInternal& circle) const override;
  void Draw() const override;
  void Move(float x, float y) override;

  [[nodiscard]] float center_x() const override { return x; }
  [[nodiscard]] float center_y() const override { return y; }

  PointInternal(const float x, const float y) : x(x), y(y) {}
  explicit PointInternal(std::pair<float, float> p) : x(p.first), y(p.second) {}
  bool operator==(const PointInternal& v) const { return x == v.x && y == v.y; }
  /*
   * Returns true if current point is located on the lower-left side of the
   * `other` point. Does not return `true` if they are the same points.
   */
  [[nodiscard]] bool IsLowerLeft(const PointInternal& other) const;
  [[nodiscard]] float Distance(const PointInternal& other) const;

  float x;
  float y;
};

struct LineInternal final : Shape {
  enum class Aligned { X, Y, NOT };

  [[nodiscard]] bool Collides(const PointInternal& point) const override;
  [[nodiscard]] bool Collides(const LineInternal& other_line) const override;
  [[nodiscard]] bool Collides(
      const RectangleInternal& rectangle) const override;
  [[nodiscard]] bool Collides(const CircleInternal& circle) const override;
  [[nodiscard]] Vector Reflect(const Vector& vec) const;

  void Draw() const override;
  void Move(float x, float y) override;

  [[nodiscard]] float center_x() const override {
    return (this->a.x + this->b.x) / 2.0;
  }
  [[nodiscard]] float center_y() const override {
    return (this->a.y + this->b.y) / 2.0;
  }

  LineInternal(const PointInternal& a, const PointInternal& b) : a(a), b(b) {
    if (std::abs(this->a.x - this->b.x) <= eps) {
      axis_aligned = Aligned::Y;
    }
    if (std::abs(this->a.y - this->b.y) <= eps) {
      axis_aligned = Aligned::X;
    }
  }
  // Transforms a line into normalized vector towards (0, 0).
  [[nodiscard]] Vector MakeVector() const {
    return {this->b.x - this->a.x, this->b.y - this->a.y};
  }
  [[nodiscard]] bool IsOnLine(const PointInternal& p) const;

  PointInternal a;
  PointInternal b;
  Aligned axis_aligned = Aligned::NOT;
};

// TODO(f1lo): Make this a class and make sure that invariants hold. i.e.
// ordered vertices and 90 degree angles.
struct RectangleInternal final : Shape {
  [[nodiscard]] bool Collides(const PointInternal& point) const override;
  [[nodiscard]] bool Collides(const LineInternal& line) const override;
  [[nodiscard]] bool Collides(
      const RectangleInternal& other_rectangle) const override;
  [[nodiscard]] bool Collides(const CircleInternal& circle) const override;
  void Draw() const override;
  void Move(float x, float y) override;

  [[nodiscard]] float center_x() const override { return (a.x + c.x) / 2.0; }
  [[nodiscard]] float center_y() const override { return (a.y + c.y) / 2.0; }

  RectangleInternal(const PointInternal& bottom_left,
                    const PointInternal& top_right)
      : a(bottom_left),
        b(bottom_left.x, top_right.y),
        c(top_right),
        d(top_right.x, bottom_left.y) {
    CHECK(bottom_left.x < top_right.x && bottom_left.y > top_right.y)
        << "Non-axis aligned rectangle is not implemented, have: "
        << absl::Substitute("($0, $1)", bottom_left.x, bottom_left.y)
        << absl::Substitute(", ($0 $1)", top_right.x, top_right.y);
  }

  PointInternal a;
  PointInternal b;
  PointInternal c;
  PointInternal d;
};

struct CircleInternal final : Shape {
  [[nodiscard]] bool Collides(const PointInternal& point) const override;
  [[nodiscard]] bool Collides(const LineInternal& line) const override;
  [[nodiscard]] bool Collides(
      const RectangleInternal& rectangle) const override;
  [[nodiscard]] bool Collides(
      const CircleInternal& other_circle) const override;
  void Draw() const override;
  void Move(float x, float y) override;

  [[nodiscard]] float center_x() const override { return a.x; }
  [[nodiscard]] float center_y() const override { return a.y; }

  CircleInternal(const PointInternal& a, const float r) : a(a), r(r) {
    CHECK(this->r > 0) << "Negative radius for circle: " << this->r;
  }

  PointInternal a;
  float r;
};

bool Clockwise(const PointInternal& a, const PointInternal& b,
               const PointInternal& c);
bool CounterClockwise(const PointInternal& a, const PointInternal& b,
                      const PointInternal& c);

}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_GEOMETRY_SHAPE_H
