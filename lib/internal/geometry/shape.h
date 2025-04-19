#ifndef LIB_INTERNAL_GEOMETRY_SHAPE_H
#define LIB_INTERNAL_GEOMETRY_SHAPE_H

#include <cstdint>
#include <memory>

#include "vec.h"

namespace lib {
namespace internal {

struct Point;
struct Line;
struct Rectangle;
struct Circle;

struct Shape {
  virtual bool Collides(const Point& point) const = 0;
  virtual bool Collides(const Line& line) const = 0;
  virtual bool Collides(const Rectangle& rectangle) const = 0;
  virtual bool Collides(const Circle& circle) const = 0;
  virtual void Draw() const = 0;
  virtual void Move(int x, int y) = 0;

  virtual ~Shape() = default;
};

struct Point final : Shape {
  bool Collides(const Point& other_point) const override;
  bool Collides(const Line& line) const override;
  bool Collides(const Rectangle& rectangle) const override;
  bool Collides(const Circle& circle) const override;
  void Draw() const override;
  void Move(int x, int y) override;

  Point(const int x, const int y) : x(x), y(y) {}
  explicit Point(std::pair<int, int> p) : x(p.first), y(p.second) {}
  bool operator==(const Point& v) const { return x == v.x && y == v.y; }
  /*
   * Returns true if current point is located on the lower-left side of the
   * `other` point. Does not return `true` if they are the same points.
   */
  [[nodiscard]] bool IsLowerLeft(const Point& other) const;
  [[nodiscard]] double Distance(const Point& other) const;

  int x;
  int y;
};

struct Line final : Shape {
  bool Collides(const Point& point) const override;
  bool Collides(const Line& other_line) const override;
  bool Collides(const Rectangle& rectangle) const override;
  bool Collides(const Circle& circle) const override;
  void Draw() const override;
  void Move(int x, int y) override;

  Line(Point a, Point b) : a(std::move(a)), b(std::move(b)) {}
  // Transforms a line into normalized vector towards (0, 0).
  [[nodiscard]] Vector MakeVector() const {
    return {this->b.x - this->a.x, this->b.y - this->a.y};
  }
  [[nodiscard]] bool IsOnLine(const Point& p) const;

  Point a;
  Point b;
};

// TODO(f1lo): Make this a class and make sure that invariants hold. i.e.
// ordered vertices and 90 degree angles.
struct Rectangle final : Shape {
  bool Collides(const Point& point) const override;
  bool Collides(const Line& line) const override;
  bool Collides(const Rectangle& other_rectangle) const override;
  bool Collides(const Circle& circle) const override;
  void Draw() const override;
  void Move(int x, int y) override;

  Rectangle(Point a, Point b, Point c, Point d)
      : a(std::move(a)), b(std::move(b)), c(std::move(c)), d(std::move(d)) {}

  Point a;
  Point b;
  Point c;
  Point d;
};

struct Circle final : Shape {
  bool Collides(const Point& point) const override;
  bool Collides(const Line& line) const override;
  bool Collides(const Rectangle& rectangle) const override;
  bool Collides(const Circle& other_circle) const override;
  void Draw() const override;
  void Move(int x, int y) override;

  Circle(Point a, const uint32_t r) : a(std::move(a)), r(r) {}

  Point a;
  uint32_t r;
};

bool Clockwise(const Point& a, const Point& b, const Point& c);
bool CounterClockwise(const Point& a, const Point& b, const Point& c);

}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_GEOMETRY_SHAPE_H
