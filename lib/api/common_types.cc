#include "lib/api/common_types.h"

#include <cmath>

inline constexpr double kEps = 1e-9;

namespace lib {
namespace api {

bool ScreenPosition::operator==(const ScreenPosition& other) const {
  return abs(x - other.x) <= kEps && abs(y - other.y) <= kEps;
}

std::ostream& operator<<(std::ostream& os, const ScreenPosition& pos) {
  os << "ScreenPosition (" << pos.x << ", " << pos.y << ")\n";
  return os;
}

bool WorldPosition::operator==(const WorldPosition& other) const {
  return abs(x - other.x) <= kEps && abs(y - other.y) <= kEps;
}

std::ostream& operator<<(std::ostream& os, const WorldPosition& pos) {
  os << "WorldPosition (" << pos.x << ", " << pos.y << ")\n";
  return os;
}

}  // namespace api
}  // namespace lib
