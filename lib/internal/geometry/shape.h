#ifndef LIB_INTERNAL_GEOMETRY_SHAPE_H
#define LIB_INTERNAL_GEOMETRY_SHAPE_H

#include <memory>

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

  virtual ~Shape() = default;
  double center_x;
  double center_y;
};

struct Point final : Shape {
  [[nodiscard]] bool Collides(const Point& other_point) const override;
  [[nodiscard]] bool Collides(const Line& line) const override;
  [[nodiscard]] bool Collides(const Rectangle& rectangle) const override;
  [[nodiscard]] bool Collides(const Circle& circle) const override;
  void Draw() const override;
  void Move(double x, double y) override;

  Point(const double x, const double y) : x(x), y(y) {
    center_x = x;
    center_y = y;
  }
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

  Line(Point a, Point b) : a(std::move(a)), b(std::move(b)) {
    if (abs(this->a.x - this->b.x) <= eps) {
      axis_aligned = Aligned::Y;
    }
    if (abs(this->a.y - this->b.y) <= eps) {
      axis_aligned = Aligned::X;
    }
    center_x = (this->a.x + this->b.x) / 2.0;
    center_y = (this->a.y + this->b.y) / 2.0;
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

  Rectangle(const Point& bottom_left, const Point& top_right)
      : a(bottom_left),
        b(bottom_left.x, top_right.y),
        c(top_right),
        d(top_right.x, bottom_left.y) {
    CHECK(bottom_left.x < top_right.x && bottom_left.y > top_right.y)
        << "Non-axis aligned rectangle is not implemented, have: "
        << absl::Substitute("($0, $1)", bottom_left.x, bottom_left.y)
        << absl::Substitute(", ($0 $1)", top_right.x, top_right.y);
    center_x = (bottom_left.x + top_right.x) / 2.0;
    center_y = (bottom_left.y + top_right.y) / 2.0;
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

  Circle(Point a, const double r) : a(std::move(a)), r(r) {
    CHECK(r > 0) << "Negative radius for circle: " << r;
    center_x = a.x;
    center_y = a.y;
  }

  Point a;
  double r;
};

bool Clockwise(const Point& a, const Point& b, const Point& c);
bool CounterClockwise(const Point& a, const Point& b, const Point& c);

}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_GEOMETRY_SHAPE_H
