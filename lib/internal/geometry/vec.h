#ifndef LIB_INTERNAL_GEOMETRY_VEC_H
#define LIB_INTERNAL_GEOMETRY_VEC_H

#include <cmath>
#include <limits>
#include <ostream>

namespace lib {
namespace internal {

/*
 * A class to define basic vector arithmetics.
 */
struct Vector {
  Vector(double x, double y) : x(x), y(y) {}
  Vector(int x, int y) : x(x), y(y) {}

  bool operator==(const Vector &other) const {
    return std::abs(x - other.x) <= std::numeric_limits<double>::epsilon() &&
           std::abs(y - other.y) <= std::numeric_limits<double>::epsilon();
  }

  Vector Project(const Vector &other) const;
  double ScalarProduct(const Vector &other) const;
  bool IsAxisAligned() const;
  double Square() const;
  double Length() const;
  Vector Multiply(double x) const;

  friend void PrintTo(const Vector &v, std::ostream *os) {
    *os << "Vector{" << v.x << "," << v.y << "}";
  }

  double x;
  double y;
};

} // namespace internal
} // namespace lib

#endif // LIB_INTERNAL_GEOMETRY_VEC_H
