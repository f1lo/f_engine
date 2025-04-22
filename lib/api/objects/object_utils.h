#ifndef LIB_API_OBJECTS_OBJECT_UTILS_H
#define LIB_API_OBJECTS_OBJECT_UTILS_H

#include <list>
#include <memory>
#include <optional>

#include "lib/api/objects/object.h"
#include "raylib/include/raylib.h"

namespace lib {
namespace api {
namespace objects {

internal::HitBox CreateCircle(double x, double y, double radius);

internal::HitBox CreateHitBoxOrDie(
    const std::vector<std::pair<double, double>>& vertices);
}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_UTILS_H
