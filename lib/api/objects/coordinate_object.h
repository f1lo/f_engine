#ifndef LIB_API_OBJECTS_COORDINATE_OBJECT_H
#define LIB_API_OBJECTS_COORDINATE_OBJECT_H

#include <list>
#include <memory>
#include <optional>

#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

static constexpr double kAxisOffset = 60;
static constexpr double kNumbersOffsetX = 30;
static constexpr double kNumbersOffsetY = 5;
static constexpr double kFontSize = 20;
static constexpr double kDistanceBetweenGrid = 200;

class CoordinateObject : public Object {
 public:
  static std::unique_ptr<CoordinateObject> MakeX(double screen_width,
                                                 double screen_height);
  static std::unique_ptr<CoordinateObject> MakeY(double screen_width,
                                                 double screen_height);

  void ReAdjustToScreen(WorldPosition screen_top_left_pos, double screen_width,
                        double screen_height);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;

 private:
  CoordinateObject(ScreenPosition screen_position_start,
                   ScreenPosition screen_position_end, double screen_width,
                   double screen_height, bool is_x_axis);

  bool OnCollisionCallback(Object& other_object) override;
  void Draw() const override;
  void DrawX() const;
  void DrawY() const;

  bool is_x_axis_;
  WorldPosition screen_top_left_pos_;
  const double screen_width_;
  const double screen_height_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_COORDINATE_OBJECT_H
