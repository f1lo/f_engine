#include "raylib/include/raylib.h"

#include "shape.h"

#include <cmath>
#include <iostream>

#include "absl/log/check.h"
#include "absl/log/log.h"
#include "absl/strings/internal/str_format/extension.h"
#include "absl/strings/str_format.h"

namespace lib {
namespace internal {

namespace {
float square(const float& a) {
  return a * a;
}
}  // namespace

bool Clockwise(const PointInternal& a, const PointInternal& b,
               const PointInternal& c) {
  return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

bool CounterClockwise(const PointInternal& a, const PointInternal& b,
                      const PointInternal& c) {
  return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}

// Draw methods.
void PointInternal::Draw() const {
  DrawPixelV(Vector2(x, y), RED);
}
void LineInternal::Draw() const {
  DrawLineEx(Vector2(a.x, a.y), Vector2(b.x, b.y),
             /*thick=*/10.0, RED);
}
void RectangleInternal::Draw() const {
  DrawRectangleLines(static_cast<int>(b.x), static_cast<int>(b.y),
                     /*width=*/static_cast<int>(c.x - b.x),
                     /*height*/ static_cast<int>(a.y - b.y), RED);
}
void CircleInternal::Draw() const {
  DrawCircleLinesV(Vector2(a.x, a.y), /*radius*/ r, RED);
}

Vector LineInternal::Reflect(const Vector& vec) const {
  CHECK(axis_aligned != Aligned::NOT)
      << "LineInternal::Reflect not implemented for non axis-aligned lines";
  if (axis_aligned == Aligned::X) {
    return {vec.x, -vec.y};
  }

  return {-vec.x, vec.y};
}

// Move methods.
void PointInternal::Move(const float xx, const float yy) {
  this->x += xx;
  this->y += yy;
}
void LineInternal::Move(const float x, const float y) {
  this->a.x += x;
  this->a.y += y;
  this->b.x += x;
  this->b.y += y;
}
void RectangleInternal::Move(const float x, const float y) {
  this->a.x += x;
  this->a.y += y;
  this->b.x += x;
  this->b.y += y;
  this->c.x += x;
  this->c.y += y;
  this->d.x += x;
  this->d.y += y;
}
void CircleInternal::Move(const float x, const float y) {
  this->a.x += x;
  this->a.y += y;
}

// POINT COLLISION
bool PointInternal::Collides(const PointInternal& other_point) const {
  return *this == other_point;
}
bool PointInternal::Collides(const LineInternal& line) const {
  return line.IsOnLine(*this);
}
bool PointInternal::Collides(const RectangleInternal& rectangle) const {
  const Vector a = LineInternal(rectangle.a, *this).MakeVector();
  const Vector b = LineInternal(rectangle.a, rectangle.b).MakeVector();
  const Vector c = LineInternal(rectangle.a, rectangle.d).MakeVector();

  return (0 <= a.DotProduct(b) && a.DotProduct(b) <= b.DotProduct(b)) &&
         (0 <= a.DotProduct(c) && a.DotProduct(c) <= c.DotProduct(c));
}
bool PointInternal::Collides(const CircleInternal& circle) const {
  return circle.r >= this->Distance(circle.a);
}

bool PointInternal::IsLowerLeft(const PointInternal& other) const {
  return this->x <= other.x && this->y < other.y;
}

float PointInternal::Distance(const PointInternal& other) const {
  return sqrt(square(this->x - other.x) + square(this->y - other.y));
}

bool LineInternal::IsOnLine(const PointInternal& p) const {
  // Check if it is on the infinite line first.
  if ((a.x - p.x) * (p.y - b.y) != (p.x - b.x) * (a.y - p.y)) {
    return false;
  }

  return std::abs(a.Distance(p) + b.Distance(p) - a.Distance(b)) < eps;
}

// LINE COLLISION.
bool LineInternal::Collides(const PointInternal& point) const {
  return point.Collides(*this);
}
// TODO(f1lo): This is incorrectly handling lines which extend each other.
bool LineInternal::Collides(const LineInternal& other_line) const {
  return CounterClockwise(this->a, other_line.a, other_line.b) !=
             CounterClockwise(this->b, other_line.a, other_line.b) &&
         CounterClockwise(this->a, this->b, other_line.a) !=
             CounterClockwise(this->a, this->b, other_line.b);
}
bool LineInternal::Collides(const RectangleInternal& rectangle) const {
  // Check intersection with individual edges first.
  // Note rectangle vertices should be ordered.
  if (this->Collides(LineInternal(rectangle.a, rectangle.b)) ||
      this->Collides(LineInternal(rectangle.b, rectangle.c)) ||
      this->Collides(LineInternal(rectangle.c, rectangle.d)) ||
      this->Collides(LineInternal(rectangle.d, rectangle.a))) {
    return true;
  }

  // If edges did not intersect the only case remaining is that the vertices of
  // the line are inside rectangle.
  // At this point either both of the vertices are inside the rectangle or none
  // of them.
  if (this->a.Collides(rectangle) ^ this->b.Collides(rectangle)) {
    return false;
  }
  return this->a.Collides(rectangle) && this->b.Collides(rectangle);
}
bool LineInternal::Collides(const CircleInternal& circle) const {
  // If one of the points is inside circle collision has happened.
  if (this->a.Collides(circle) || this->b.Collides(circle)) {
    return true;
  }

  float t =
      ((circle.a.x - a.x) * (b.x - a.x) + (circle.a.y - a.y) * (b.y - a.y)) /
      ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
  if (t < 0 || t > 1) {
    return false;
  }

  PointInternal closest =
      PointInternal{a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
  return closest.Distance(circle.a) - eps <= circle.r;
}

// RECTANGLE COLLISION.
bool RectangleInternal::Collides(const PointInternal& point) const {
  return point.Collides(*this);
}
bool RectangleInternal::Collides(const LineInternal& line) const {
  return line.Collides(*this);
}
bool RectangleInternal::Collides(
    const RectangleInternal& other_rectangle) const {
  return this->a.x <= other_rectangle.c.x && this->c.x >= other_rectangle.a.x &&
         this->a.y >= other_rectangle.c.y && this->c.y <= other_rectangle.a.y;
}

bool RectangleInternal::Collides(const CircleInternal& circle) const {
  // Check if the center is inside rectangle.
  if (this->Collides(circle.a)) {
    return true;
  }

  // Otherwise check for circle collision with the rectangle sides one by one.
  return circle.Collides(LineInternal(this->a, this->b)) ||
         circle.Collides(LineInternal(this->b, this->c)) ||
         circle.Collides(LineInternal(this->c, this->d)) ||
         circle.Collides(LineInternal(this->d, this->a));
}

// CIRCLE COLLISION.
bool CircleInternal::Collides(const PointInternal& point) const {
  return point.Collides(*this);
}
bool CircleInternal::Collides(const LineInternal& line) const {
  return line.Collides(*this);
}
bool CircleInternal::Collides(const RectangleInternal& rectangle) const {
  return rectangle.Collides(*this);
}
bool CircleInternal::Collides(const CircleInternal& other_circle) const {
  return this->a.Distance(other_circle.a) <= this->r + other_circle.r;
}

}  // namespace internal
}  // namespace lib
