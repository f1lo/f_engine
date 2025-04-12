#include "raylib/include/raylib.h"

#include "shape.h"

#include "absl/log/log.h"
#include <cmath>
#include <limits>

namespace lib {
namespace internal {

namespace {
int square(const int &a) { return a * a; }
} // namespace

bool Clockwise(const Point &a, const Point &b, const Point &c) {
  // Avoid overflow.
  return static_cast<long long>(a.x) * (b.y - c.y) +
             static_cast<long long>(b.x) * (c.y - a.y) +
             static_cast<long long>(c.x) * (a.y - b.y) <
         0;
}

bool CounterClockwise(const Point &a, const Point &b, const Point &c) {
  // Avoid overflow.
  return static_cast<long long>(a.x) * (b.y - c.y) +
             static_cast<long long>(b.x) * (c.y - a.y) +
             static_cast<long long>(c.x) * (a.y - b.y) >
         0;
}

// Draw methods.
void Point::Draw() const { DrawPixel(x, y, RED); }
void Line::Draw() const { DrawLine(a.x, a.y, b.x, b.y, RED); }
void Rectangle::Draw() const {
  DrawRectangleLines(a.x, a.y, /*width=*/c.x - b.x, /*height*/ b.y - a.y, RED);
}
void Circle::Draw() const {
  DrawCircleLines(a.x, a.y, /*radius*/ static_cast<float>(r), RED);
}

// POINT COLLISION
bool Point::Collides(const Point &other_point) const {
  return *this == other_point;
}
bool Point::Collides(const Line &line) const { return line.IsOnLine(*this); }
bool Point::Collides(const Rectangle &rectangle) const {
  const Vector a = Line(rectangle.a, *this).MakeVector();
  const Vector b = Line(rectangle.a, rectangle.b).MakeVector();
  const Vector c = Line(rectangle.a, rectangle.d).MakeVector();

  return (0 <= a.ScalarProduct(b) &&
          a.ScalarProduct(b) <= b.ScalarProduct(b)) &&
         (0 <= a.ScalarProduct(c) && a.ScalarProduct(c) <= c.ScalarProduct(c));
}
bool Point::Collides(const Circle &circle) const {
  return static_cast<double>(circle.r) >= this->Distance(circle.a);
}

bool Point::IsLowerLeft(const Point &other) const {
  return this->x <= other.x && this->y < other.y;
}

double Point::Distance(const Point &other) const {
  return sqrt(square(this->x - other.x) + square(this->y - other.y));
}

bool Line::IsOnLine(const Point &p) const {
  // Check if it is on the infinite line first.
  if ((a.x - p.x) * (p.y - b.y) != (p.x - b.x) * (a.y - p.y)) {
    return false;
  }

  return std::abs(a.Distance(p) + b.Distance(p) - a.Distance(b)) <
         std::numeric_limits<double>::epsilon();
}

// LINE COLLISION.
bool Line::Collides(const Point &point) const { return point.Collides(*this); }
// TODO(f1lo): This is incorrectly handling lines which extend each other.
bool Line::Collides(const Line &other_line) const {
  return CounterClockwise(this->a, other_line.a, other_line.b) !=
             CounterClockwise(this->b, other_line.a, other_line.b) &&
         CounterClockwise(this->a, this->b, other_line.a) !=
             CounterClockwise(this->a, this->b, other_line.b);
}
bool Line::Collides(const Rectangle &rectangle) const {
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
  bool vert_a_collides = this->a.Collides(rectangle);
  bool vert_b_collides = this->b.Collides(rectangle);
  // At this point either both of the vertices are inside the rectangle or none
  // of them.
  if (vert_a_collides ^ vert_b_collides) {
    LOG(ERROR) << "Line::Collides() segment points mismatch";
    return false;
  }
  return this->a.Collides(rectangle) && this->b.Collides(rectangle);
}
bool Line::Collides(const Circle &circle) const {
  // If one of the points is inside circle collision has happened.
  if (this->a.Collides(circle) || this->b.Collides(circle)) {
    return true;
  }

  // Project the vector connecting one of the point of the line to the center
  // onto the line itself.
  // Iff this projection falls inside the line AND the distance from the center
  // to the projected point on the line is less than or equal to the circle
  // radius.
  const Vector line_vector = this->MakeVector();
  const Vector line_point_to_circle_center_vector =
      Line(this->a, circle.a).MakeVector();
  const Vector projection =
      line_vector.Project(line_point_to_circle_center_vector);
  // `line_vector` and `projection` always face the same direction, if length of
  // `projection` is shorter it means projected point falls onto the line.
  if (projection.Length() > line_vector.Length()) {
    return false;
  }
  // Now the only thing left is to check is distance.
  // If the distance from the circle center to the line is less than the radius
  // segment intersects the circle.
  return line_point_to_circle_center_vector.Square() - projection.Square() <=
         circle.r * circle.r;
}

// RECTANGLE COLLISION.
bool Rectangle::Collides(const Point &point) const {
  return point.Collides(*this);
}
bool Rectangle::Collides(const Line &line) const {
  return line.Collides(*this);
}
bool Rectangle::Collides(const Rectangle &other_rectangle) const {
  int rect_a_x1 = this->a.x;
  int rect_a_x2 = this->c.x;
  int rect_a_y1 = this->a.y;
  int rect_a_y2 = this->c.y;
  if (rect_a_x1 > rect_a_x2) {
    std::swap(rect_a_x1, rect_a_x2);
  }
  if (rect_a_y1 > rect_a_y2) {
    std::swap(rect_a_y1, rect_a_y2);
  }

  int rect_b_x1 = other_rectangle.a.x;
  int rect_b_x2 = other_rectangle.c.x;
  int rect_b_y1 = other_rectangle.a.y;
  int rect_b_y2 = other_rectangle.c.y;
  if (rect_b_x1 > rect_b_x2) {
    std::swap(rect_b_x1, rect_b_x2);
  }
  if (rect_b_y1 > rect_b_y2) {
    std::swap(rect_b_y1, rect_b_y2);
  }

  return rect_a_x1 <= rect_b_x2 && rect_a_x2 >= rect_b_x1 &&
         rect_a_y1 <= rect_b_y2 && rect_a_y2 >= rect_b_y1;
}

bool Rectangle::Collides(const Circle &circle) const {
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
bool Circle::Collides(const Point &point) const {
  return point.Collides(*this);
}
bool Circle::Collides(const Line &line) const { return line.Collides(*this); }
bool Circle::Collides(const Rectangle &rectangle) const {
  return rectangle.Collides(*this);
}
bool Circle::Collides(const Circle &other_circle) const {
  return this->a.Distance(other_circle.a) <= this->r + other_circle.r;
}

} // namespace internal
} // namespace lib
