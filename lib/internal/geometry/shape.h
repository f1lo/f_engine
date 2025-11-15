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

struct Point;
struct Line;
struct Rectangle;
struct Circle;

struct Shape {
  [[nodiscard]] virtual bool Collides(const Point& point) const = 0;
  [[nodiscard]] virtual bool Collides(const Line& line) const = 0;
  [[nodiscard]] virtual bool Collides(const Rectangle& rectangle) const = 0;
  [[nodiscard]] virtual bool Collides(const Circle& circle) const = 0;
  virtual void Draw() const = 0;
  virtual void Move(double x, double y) = 0;

  [[nodiscard]] virtual double center_x() const = 0;
  [[nodiscard]] virtual double center_y() const = 0;

  virtual ~Shape() = default;
};

struct Point final : Shape {
  [[nodiscard]] bool Collides(const Point& other_point) const override;
  [[nodiscard]] bool Collides(const Line& line) const override;
  [[nodiscard]] bool Collides(const Rectangle& rectangle) const override;
  [[nodiscard]] bool Collides(const Circle& circle) const override;
  void Draw() const override;
  void Move(double x, double y) override;

  [[nodiscard]] double center_x() const override { return x; }
  [[nodiscard]] double center_y() const override { return y; }

  Point(const double x, const double y) : x(x), y(y) {}
  explicit Point(std::pair<double, double> p) : x(p.first), y(p.second) {}
  bool operator==(const Point& v) const { return x == v.x && y == v.y; }
  /*
   * Returns true if current point is located on the lower-left side of the
   * `other` point. Does not return `true` if they are the same points.
   */
  [[nodiscard]] bool IsLowerLeft(const Point& other) const;
  [[nodiscard]] double Distance(const Point& other) const;

  double x;
  double y;
};

struct Line final : Shape {
  enum class Aligned { X, Y, NOT };

  [[nodiscard]] bool Collides(const Point& point) const override;
  [[nodiscard]] bool Collides(const Line& other_line) const override;
  [[nodiscard]] bool Collides(const Rectangle& rectangle) const override;
  [[nodiscard]] bool Collides(const Circle& circle) const override;
  [[nodiscard]] Vector Reflect(const Vector& vec) const;

  void Draw() const override;
  void Move(double x, double y) override;

  [[nodiscard]] double center_x() const override {
    return (this->a.x + this->b.x) / 2.0;
  }
  [[nodiscard]] double center_y() const override {
    return (this->a.y + this->b.y) / 2.0;
  }

  Line(const Point& a, const Point& b) : a(a), b(b) {
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
  [[nodiscard]] bool IsOnLine(const Point& p) const;

  Point a;
  Point b;
  Aligned axis_aligned = Aligned::NOT;
};

// TODO(f1lo): Make this a class and make sure that invariants hold. i.e.
// ordered vertices and 90 degree angles.
struct Rectangle final : Shape {
  [[nodiscard]] bool Collides(const Point& point) const override;
  [[nodiscard]] bool Collides(const Line& line) const override;
  [[nodiscard]] bool Collides(const Rectangle& other_rectangle) const override;
  [[nodiscard]] bool Collides(const Circle& circle) const override;
  void Draw() const override;
  void Move(double x, double y) override;

  [[nodiscard]] double center_x() const override { return (a.x + c.x) / 2.0; }
  [[nodiscard]] double center_y() const override { return (a.y + c.y) / 2.0; }

  Rectangle(const Point& bottom_left, const Point& top_right)
      : a(bottom_left),
        b(bottom_left.x, top_right.y),
        c(top_right),
        d(top_right.x, bottom_left.y) {
    CHECK(bottom_left.x < top_right.x && bottom_left.y > top_right.y)
        << "Non-axis aligned rectangle is not implemented, have: "
        << absl::Substitute("($0, $1)", bottom_left.x, bottom_left.y)
        << absl::Substitute(", ($0 $1)", top_right.x, top_right.y);
  }

  Point a;
  Point b;
  Point c;
  Point d;
};

struct Circle final : Shape {
  [[nodiscard]] bool Collides(const Point& point) const override;
  [[nodiscard]] bool Collides(const Line& line) const override;
  [[nodiscard]] bool Collides(const Rectangle& rectangle) const override;
  [[nodiscard]] bool Collides(const Circle& other_circle) const override;
  void Draw() const override;
  void Move(double x, double y) override;

  [[nodiscard]] double center_x() const override { return a.x; }
  [[nodiscard]] double center_y() const override { return a.y; }

  Circle(const Point& a, const double r) : a(a), r(r) {
    CHECK(this->r > 0) << "Negative radius for circle: " << this->r;
  }

  Point a;
  double r;
};

bool Clockwise(const Point& a, const Point& b, const Point& c);
bool CounterClockwise(const Point& a, const Point& b, const Point& c);

}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_GEOMETRY_SHAPE_H
