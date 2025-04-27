#include "vec.h"

#include "absl/log/check.h"

namespace lib {
namespace internal {

double Vector::DotProduct(const Vector& other) const {
  return this->x * other.x + this->y * other.y;
}

bool Vector::IsAxisAligned() const {
  return std::abs(this->x) <= eps || std::abs(this->y) <= eps;
}

double Vector::Square() const {
  return x * x + y * y;
}

double Vector::Length() const {
  return std::sqrt(Square());
}

Vector Vector::Multiply(const double x) const {
  return {this->x * x, this->y * x};
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

double Vector::Angle(const Vector& other) const {
  // Formula is:
  // this * other = |this| * |other| * cos(a).
  //           this * other
  // cos(a) = ----------------
  //          |this| * |other|
  // Where |x| is the length of vector x, and x * y is dot product of vectors x
  // and y.
  return acos(this->DotProduct(other) / (this->Length() * other.Length()));
}

Vector Vector::Rotate(const double& angle) const {
  return Vector(x * cos(angle) - y * sin(angle),
                x * sin(angle) + y * cos(angle));
}

}  // namespace internal
}  // namespace lib
