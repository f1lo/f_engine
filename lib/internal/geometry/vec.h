#ifndef LIB_INTERNAL_GEOMETRY_VEC_H
#define LIB_INTERNAL_GEOMETRY_VEC_H

#include <cmath>

namespace lib {
namespace internal {

inline constexpr float eps = 1e-5f;

/*
 * A class to define basic vector arithmetics.
 */
struct Vector {
  bool operator==(const Vector& other) const {
    return std::abs(x - other.x) <= eps && std::abs(y - other.y) <= eps;
  }

  [[nodiscard]] Vector Project(const Vector& other) const;
  [[nodiscard]] float DotProduct(const Vector& other) const;
  [[nodiscard]] bool IsAxisAligned() const;
  [[nodiscard]] float Square() const;
  [[nodiscard]] float Length() const;
  [[nodiscard]] Vector Multiply(float xx) const;
  [[nodiscard]] float Angle(const Vector& other) const;
  [[nodiscard]] Vector Rotate(const float& angle) const;
  [[nodiscard]] Vector ToUnitVector() const;
  [[nodiscard]] bool IsZero() const;

  float x;
  float y;
};

}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_GEOMETRY_VEC_H
