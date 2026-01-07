#ifndef LIB_API_OBJECTS_OBJECT_UTILS_H
#define LIB_API_OBJECTS_OBJECT_UTILS_H

#include <vector>

#include "lib/api/common_types.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {

internal::HitBox CreateCircle(float x, float y, float radius);
internal::HitBox CreateHitBoxOrDie(
    const std::vector<std::pair<float, float>>& vertices);
internal::HitBox CreateHitBoxOrDie(const std::vector<ScreenPosition>& vertices);

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_UTILS_H
