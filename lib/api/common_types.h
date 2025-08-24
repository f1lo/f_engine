#ifndef LIB_API_COMMON_TYPES_H
#define LIB_API_COMMON_TYPES_H

#include <iostream>

namespace lib {
namespace api {

struct ScreenPosition {
  double x;
  double y;

  bool operator==(const ScreenPosition& other) const;
  bool operator!=(const ScreenPosition& other) const;
  friend std::ostream& operator<<(std::ostream& os, const ScreenPosition& pos);
};

struct WorldPosition {
  double x;
  double y;

  bool operator==(const WorldPosition& other) const;
  bool operator!=(const WorldPosition& other) const;
  friend std::ostream& operator<<(std::ostream& os, const WorldPosition& pos);
};

}  // namespace api
}  // namespace lib

#endif  //LIB_API_COMMON_TYPES_H
