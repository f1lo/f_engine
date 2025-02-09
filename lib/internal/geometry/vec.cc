#include "vec.h"

#include <cmath>
#include <limits>

namespace lib {
namespace internal {

double Vector::ScalarProduct(const Vector &other) const {
  return this->x * other.x + this->y * other.y;
}

bool Vector::IsAxisAligned() const {
  return std::abs(this->x) <= std::numeric_limits<double>::epsilon() ||
         std::abs(this->y) <= std::numeric_limits<double>::epsilon();
}

double Vector::Square() const { return this->x * this->x + this->y * this->y; }

double Vector::Length() const { return std::sqrt(this->Square()); }

Vector Vector::Multiply(const double x) const {
  return {this->x * x, this->y * x};
}

// TODO(f1lo): This can divide on 0 in case vector with 0 length is provided.
// Returning status from here or from constructor is cumbersome for now - but
// for the future consider either not allowing 0 sized vectors or make sure that
// `Project` errors out in case current vector has 0 length.
Vector Vector::Project(const Vector &other) const {
  // Formula is:
  // ((`this` * `other`) / (`this` * `this`)) * `this`;
  // Where `this` is the vector on which `other` is being projected.
  Vector v = this->Multiply(this->ScalarProduct(other));
  return {v.x / this->Square(), v.y / this->Square()};
}

} // namespace internal
} // namespace lib
