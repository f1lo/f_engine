#include "lib/api/objects/object_utils.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/common_types.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {
namespace {

using ::testing::HasSubstr;

class ObjectUtilsTest : public ::testing::Test {};

using ObjectUtilsDeathTest = ObjectUtilsTest;

TEST(ObjectUtilsDeathTest, CreateHitBoxOrDieTrianlge) {
  EXPECT_DEATH(CreateHitBoxOrDie(std::vector<std::pair<float, float>>{
                   {0, 0}, {1, 1}, {2, 0}}),
               HasSubstr("HitBox::CreateHitBox() failed"));
}

TEST(ObjectUtilsDeathTest, CreateHitBoxOrDieScreenPosition) {
  EXPECT_DEATH(
      CreateHitBoxOrDie(std::vector<ScreenPosition>{{0, 0}, {1, 1}, {2, 0}}),
      HasSubstr("HitBox::CreateHitBox() failed"));
}

TEST(ObjectUtilsTest, CreateCircle) {
  const internal::HitBox circle = CreateCircle(/*x=*/1, /*y=*/2, /*radius=*/5);

  EXPECT_DOUBLE_EQ(circle.center_x(), 1);
  EXPECT_DOUBLE_EQ(circle.center_y(), 2);
}

TEST(ObjectUtilsTest, CreateLine) {
  const internal::HitBox line =
      CreateHitBoxOrDie(std::vector<ScreenPosition>{{0, 0}, {0, 1}});

  EXPECT_DOUBLE_EQ(line.center_x(), 0);
  EXPECT_DOUBLE_EQ(line.center_y(), 0.5);
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib
