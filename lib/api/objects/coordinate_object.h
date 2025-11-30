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
static constexpr float kDistanceBetweenGrid = 200;

class CoordinateObject : public Object {
 public:
  static std::unique_ptr<CoordinateObject> MakeX(float screen_width,
                                                 float screen_height);
  static std::unique_ptr<CoordinateObject> MakeY(float screen_width,
                                                 float screen_height);

  void ReAdjustToScreen(WorldPosition screen_top_left_pos, float screen_width,
                        float screen_height);

  void Update(const std::list<std::unique_ptr<Object>>& other_objects) override;

 private:
  CoordinateObject(ScreenPosition screen_position_start,
                   ScreenPosition screen_position_end, float screen_width,
                   float screen_height, bool is_x_axis);

  bool OnCollisionCallback(Object& other_object) override;
  void Draw() const override;
  void DrawX() const;
  void DrawY() const;

  bool is_x_axis_;
  WorldPosition screen_top_left_pos_;
  float screen_width_;
  float screen_height_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_COORDINATE_OBJECT_H
