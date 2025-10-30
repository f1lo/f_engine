#ifndef LIB_API_OBJECTS_COORDINATE_OBJECT_H
#define LIB_API_OBJECTS_COORDINATE_OBJECT_H

#include <list>
#include <memory>

#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

static constexpr double kAxisOffset = 60;
static constexpr double kNumbersOffsetX = 30;
static constexpr double kNumbersOffsetY = 5;
static constexpr double kFontSize = 20;

class CoordinateObject : public Object {
 public:
  static std::unique_ptr<CoordinateObject> MakeX(const double screen_width);
  static std::unique_ptr<CoordinateObject> MakeY(const double screen_height);

  void ReAdjustToScreen(const WorldPosition screen_top_left_pos,
                        const double screen_width, const double screen_height);

 private:
  CoordinateObject(const ScreenPosition screen_position_start,
                   const ScreenPosition screen_position_end,
                   const bool is_x_axis);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;
  bool OnCollisionCallback(Object& other_object) override;
  void Draw() const override;

  bool is_x_axis_;
  WorldPosition screen_top_left_pos_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_COORDINATE_OBJECT_H
