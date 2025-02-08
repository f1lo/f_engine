#include "shape.h"

#include <cmath>
#include <limits>
#include <sys/stat.h>

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

// POINT COLLISION
bool Point::Collides(const Point &other_point) const {
  return *this == other_point;
}
bool Point::Collides(const Line &line) const { return line.IsOnLine(*this); }
bool Point::Collides(const Rectangle &rectangle) const {
  const Point a = Line(rectangle.a, *this).MakeVector();
  const Point b = Line(rectangle.a, rectangle.b).MakeVector();
  const Point c = Line(rectangle.a, rectangle.d).MakeVector();

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

int Point::ScalarProduct(const Point &other) const {
  return this->x * other.x + this->y * other.y;
}

bool Line::IsOnLine(const Point &p) const {
  // Check if it is on the infinite line first.
  if ((a.x - p.x) * (p.y - b.y) != (p.x - b.x) * (a.y - p.y)) {
    return false;
  }

  return std::abs(a.Distance(p) + b.Distance(p) - a.Distance(b)) <
         std::numeric_limits<double>::epsilon();
}
int Line::ScalarProduct(const Line &other) const {
  return (other.a.x - this->a.x) + this->b.y * other.b.y;
}

// LINE COLLISION.
bool Line::Collides(const Point &point) const { return point.Collides(*this); }
bool Line::Collides(const Line &other_line) const {
  return CounterClockwise(this->a, other_line.a, other_line.b) !=
             CounterClockwise(this->b, other_line.a, other_line.b) &&
         CounterClockwise(this->a, this->b, other_line.a) !=
             CounterClockwise(this->a, this->b, other_line.b);
}
bool Line::Collides(const Rectangle &rectangle) const {}
bool Line::Collides(const Circle &circle) const {}

// RECTANGLE COLLISION.
bool Rectangle::Collides(const Point &point) const {
  return point.Collides(*this);
}
bool Rectangle::Collides(const Line &line) const {
  return line.Collides(*this);
}
bool Rectangle::Collides(const Rectangle &other_rectangle) const {}
bool Rectangle::Collides(const Circle &circle) const {}

// CIRCLE COLLISION.
bool Circle::Collides(const Point &point) const {
  return point.Collides(*this);
}
bool Circle::Collides(const Line &line) const { return line.Collides(*this); }
bool Circle::Collides(const Rectangle &rectangle) const {
  return rectangle.Collides(*this);
}
bool Circle::Collides(const Circle &other_circle) const {}


} // namespace internal
} // namespace lib
