#ifndef LIB_INTERNAL_GEOMETRY_VEC_H
#define LIB_INTERNAL_GEOMETRY_VEC_H

#include <cmath>

namespace lib {
namespace internal {

inline constexpr double eps = 1e-9;

/*
 * A class to define basic vector arithmetics.
 */
struct Vector {
  bool operator==(const Vector& other) const {
    return std::abs(x - other.x) <= eps && std::abs(y - other.y) <= eps;
  }

  [[nodiscard]] Vector Project(const Vector& other) const;
  [[nodiscard]] double DotProduct(const Vector& other) const;
  [[nodiscard]] bool IsAxisAligned() const;
  [[nodiscard]] double Square() const;
  [[nodiscard]] double Length() const;
  [[nodiscard]] Vector Multiply(double x) const;
  [[nodiscard]] double Angle(const Vector& other) const;
  [[nodiscard]] Vector Rotate(const double& angle) const;
  [[nodiscard]] Vector ToUnitVector() const;
  [[nodiscard]] bool IsZero() const;

  double x;
  double y;
};

}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_GEOMETRY_VEC_H
