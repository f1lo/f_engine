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
  bool operator==(const Vector& other) const {
    return std::abs(x - other.x) <= std::numeric_limits<double>::epsilon() &&
           std::abs(y - other.y) <= std::numeric_limits<double>::epsilon();
  }

  [[nodiscard]] Vector Project(const Vector& other) const;
  [[nodiscard]] double ScalarProduct(const Vector& other) const;
  [[nodiscard]] bool IsAxisAligned() const;
  [[nodiscard]] double Square() const;
  [[nodiscard]] double Length() const;
  [[nodiscard]] Vector Multiply(double x) const;

  double x;
  double y;
};

}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_GEOMETRY_VEC_H
