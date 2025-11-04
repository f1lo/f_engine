#include "lib/api/objects/coordinate_object.h"

#include <list>
#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

TEST(CoordinateObjectTest, XAxisReadjustement) {
  const double screen_width = 100;
  std::unique_ptr<CoordinateObject> x_object =
      CoordinateObject::MakeX(screen_width);

  x_object->ReAdjustToScreen(/*screen_top_left_pos=*/{.x = 20, .y = 10},
                             screen_width, /*screen_height=*/80);

  EXPECT_EQ(
      x_object->center(),
      (WorldPosition{.x = (20.0 + screen_width + 20.0) / 2.0 + kAxisOffset,
                     .y = 10 + kAxisOffset}));
}

TEST(CoordinateObjectTest, YAxisReadjustement) {
  const double screen_height = 80;
  std::unique_ptr<CoordinateObject> y_object =
      CoordinateObject::MakeY(screen_height);

  y_object->ReAdjustToScreen(/*screen_top_left_pos=*/{.x = 20, .y = 10},
                             /*screen_width=*/100, screen_height);

  EXPECT_EQ(
      y_object->center(),
      (WorldPosition{.x = 20.0 + kAxisOffset,
                     .y = (10.0 + screen_height + 10.0) / 2.0 + kAxisOffset}));
}

TEST(CoordinateObjectTest, UpdateDoesNothing) {
  std::unique_ptr<CoordinateObject> x_object =
      CoordinateObject::MakeX(/*screen_width*/ 10);
  std::unique_ptr<CoordinateObject> y_object =
      CoordinateObject::MakeY(/*screen_height=*/20);
  std::unique_ptr<CoordinateObject> dummy_object =
      CoordinateObject::MakeX(/*screen_width*/ 30);
  std::list<std::unique_ptr<Object>> objs;
  objs.push_back(std::move(dummy_object));

  x_object->Update(objs);
  y_object->Update(objs);

  EXPECT_EQ(x_object->center(), (WorldPosition{.x = 5.0, .y = kAxisOffset}));
  EXPECT_EQ(y_object->center(), (WorldPosition{.x = kAxisOffset, .y = 10.0}));
}

}  // namespace objects
}  // namespace api
}  // namespace lib
