#include "vec.h"

#include "absl/log/check.h"

namespace lib {
namespace internal {

float Vector::DotProduct(const Vector& other) const {
  return this->x * other.x + this->y * other.y;
}

bool Vector::IsAxisAligned() const {
  return std::abs(this->x) <= eps || std::abs(this->y) <= eps;
}

float Vector::Square() const {
  return x * x + y * y;
}

float Vector::Length() const {
  return std::sqrt(Square());
}

Vector Vector::Multiply(const float xx) const {
  return {this->x * xx, this->y * xx};
}

// TODO(f1lo): This can divide on 0 in case vector with 0 length is provided.
// Returning status from here or from constructor is cumbersome for now - but
// for the future consider either not allowing 0 sized vectors or make sure that
// `Project` errors out in case current vector has 0 length.
Vector Vector::Project(const Vector& other) const {
  // Formula is:
  // ((`this` * `other`) / (`this` * `this`)) * `this`;
  // Where `this` is the vector on which `other` is being projected.
  Vector v = this->Multiply(this->DotProduct(other));
  return {v.x / this->Square(), v.y / this->Square()};
}

float Vector::Angle(const Vector& other) const {
  // Formula is:
  // this * other = |this| * |other| * cos(a).
  //           this * other
  // cos(a) = ----------------
  //          |this| * |other|
  // Where |x| is the length of vector x, and x * y is dot product of vectors x
  // and y.
  return acos(this->DotProduct(other) / (this->Length() * other.Length()));
}

Vector Vector::Rotate(const float& angle) const {
  return Vector(x * cos(angle) - y * sin(angle),
                x * sin(angle) + y * cos(angle));
}

Vector Vector::ToUnitVector() const {
  return Multiply(1.0f / Length());
}

bool Vector::IsZero() const {
  return std::abs(this->x) <= eps && std::abs(this->y) <= eps;
}

}  // namespace internal
}  // namespace lib
