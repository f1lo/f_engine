#include "raylib/include/raylib.h"

#include "shape.h"

#include <cmath>
#include <iostream>
#include <limits>

#include "absl/log/check.h"
#include "absl/log/log.h"
#include "absl/strings/internal/str_format/extension.h"
#include "absl/strings/str_format.h"

namespace lib {
namespace internal {

namespace {
double square(const double& a) {
  return a * a;
}
}  // namespace

bool Clockwise(const Point& a, const Point& b, const Point& c) {
  return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

bool CounterClockwise(const Point& a, const Point& b, const Point& c) {
  return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}

// Draw methods.
void Point::Draw() const {
  // Unsafe cast.
  DrawPixelV(Vector2(static_cast<float>(x), static_cast<float>(y)), RED);
}
void Line::Draw() const {
  // Unsafe cast.
  DrawLineV(Vector2(static_cast<float>(a.x), static_cast<float>(a.y)),
            Vector2(static_cast<float>(b.x), static_cast<float>(b.y)), RED);
}
void Rectangle::Draw() const {
  DrawRectangleLines(b.x, b.y, /*width=*/c.x - b.x, /*height*/ a.y - b.y, RED);
}
void Circle::Draw() const {
  DrawCircleLinesV(Vector2(static_cast<float>(a.x), static_cast<float>(a.y)),
                   /*radius*/ static_cast<float>(r), RED);
}

Vector Line::Reflect(const Vector& vec) const {
  CHECK(axis_aligned != Aligned::NOT)
      << "Line::Reflect not implemented for non axis-aligned lines";
  if (axis_aligned == Aligned::X) {
    return {vec.x, -vec.y};
  }

  return {-vec.x, vec.y};
}

// Move methods.
void Point::Move(double x, double y) {
  this->x += x;
  this->y += y;
}
void Line::Move(double x, double y) {
  this->a.x += x;
  this->a.y += y;
  this->b.x += x;
  this->b.y += y;
}
void Rectangle::Move(double x, double y) {
  this->a.x += x;
  this->a.y += y;
  this->b.x += x;
  this->b.y += y;
  this->c.x += x;
  this->c.y += y;
  this->d.x += x;
  this->d.y += y;
}
void Circle::Move(double x, double y) {
  this->a.x += x;
  this->a.y += y;
}

// POINT COLLISION
bool Point::Collides(const Point& other_point) const {
  return *this == other_point;
}
bool Point::Collides(const Line& line) const {
  return line.IsOnLine(*this);
}
bool Point::Collides(const Rectangle& rectangle) const {
  const Vector a = Line(rectangle.a, *this).MakeVector();
  const Vector b = Line(rectangle.a, rectangle.b).MakeVector();
  const Vector c = Line(rectangle.a, rectangle.d).MakeVector();

  return (0 <= a.DotProduct(b) && a.DotProduct(b) <= b.DotProduct(b)) &&
         (0 <= a.DotProduct(c) && a.DotProduct(c) <= c.DotProduct(c));
}
bool Point::Collides(const Circle& circle) const {
  return circle.r >= this->Distance(circle.a);
}

bool Point::IsLowerLeft(const Point& other) const {
  return this->x <= other.x && this->y < other.y;
}

double Point::Distance(const Point& other) const {
  return sqrt(square(this->x - other.x) + square(this->y - other.y));
}

bool Line::IsOnLine(const Point& p) const {
  // Check if it is on the infinite line first.
  if ((a.x - p.x) * (p.y - b.y) != (p.x - b.x) * (a.y - p.y)) {
    return false;
  }

  return std::abs(a.Distance(p) + b.Distance(p) - a.Distance(b)) < eps;
}

// LINE COLLISION.
bool Line::Collides(const Point& point) const {
  return point.Collides(*this);
}
// TODO(f1lo): This is incorrectly handling lines which extend each other.
bool Line::Collides(const Line& other_line) const {
  return CounterClockwise(this->a, other_line.a, other_line.b) !=
             CounterClockwise(this->b, other_line.a, other_line.b) &&
         CounterClockwise(this->a, this->b, other_line.a) !=
             CounterClockwise(this->a, this->b, other_line.b);
}
bool Line::Collides(const Rectangle& rectangle) const {
  // Check intersection with individual edges first.
  // Note rectangle vertices should be ordered.
  if (this->Collides(Line(rectangle.a, rectangle.b)) ||
      this->Collides(Line(rectangle.b, rectangle.c)) ||
      this->Collides(Line(rectangle.c, rectangle.d)) ||
      this->Collides(Line(rectangle.d, rectangle.a))) {
    return true;
  }

  // If edges did not intersect the only case remaining is that the vertices of
  // the line are inside rectangle.
  // At this point either both of the vertices are inside the rectangle or none
  // of them.
  if (this->a.Collides(rectangle) ^ this->b.Collides(rectangle)) {
    LOG(ERROR) << "Line::Collides() segment points mismatch";
    return false;
  }
  return this->a.Collides(rectangle) && this->b.Collides(rectangle);
}
bool Line::Collides(const Circle& circle) const {
  // If one of the points is inside circle collision has happened.
  if (this->a.Collides(circle) || this->b.Collides(circle)) {
    return true;
  }

  double t =
      ((circle.a.x - a.x) * (b.x - a.x) + (circle.a.y - a.y) * (b.y - a.y)) /
      ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
  if (t < 0 || t > 1) {
    return false;
  }

  Point closest = Point{a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
  return closest.Distance(circle.a) - eps <= circle.r;
}

// RECTANGLE COLLISION.
bool Rectangle::Collides(const Point& point) const {
  return point.Collides(*this);
}
bool Rectangle::Collides(const Line& line) const {
  return line.Collides(*this);
}
bool Rectangle::Collides(const Rectangle& other_rectangle) const {
  return this->a.x <= other_rectangle.c.x && this->c.x >= other_rectangle.a.x &&
         this->a.y >= other_rectangle.c.y && this->c.y <= other_rectangle.a.y;
}

bool Rectangle::Collides(const Circle& circle) const {
  // Check if the center is inside rectangle.
  if (this->Collides(circle.a)) {
    return true;
  }

  // Otherwise check for circle collision with the rectangle sides one by one.
  return circle.Collides(Line(this->a, this->b)) ||
         circle.Collides(Line(this->b, this->c)) ||
         circle.Collides(Line(this->c, this->d)) ||
         circle.Collides(Line(this->d, this->a));
}

// CIRCLE COLLISION.
bool Circle::Collides(const Point& point) const {
  return point.Collides(*this);
}
bool Circle::Collides(const Line& line) const {
  return line.Collides(*this);
}
bool Circle::Collides(const Rectangle& rectangle) const {
  return rectangle.Collides(*this);
}
bool Circle::Collides(const Circle& other_circle) const {
  return this->a.Distance(other_circle.a) <= this->r + other_circle.r;
}

}  // namespace internal
}  // namespace lib
