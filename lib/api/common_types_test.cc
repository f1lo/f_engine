#include "lib/api/common_types.h"

#include <sstream>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace api {
namespace {

using ::testing::HasSubstr;

class CommonTypesTest {};

using CommonTypesDeathTest = CommonTypesTest;

TEST(CommonTypesDeathTest, ViewPortContextZeroWidth) {
  EXPECT_DEATH(ViewPortContext(/*screen_width=*/0, /*screen_height=*/100,
                               /*native_screen_width=*/200,
                               /*native_screen_height=*/200),
               HasSubstr("Bad ViewPortContext, scale is 0."));
}

TEST(CommonTypesDeathTest, ViewPortContextZeroHeight) {
  EXPECT_DEATH(ViewPortContext(/*screen_width=*/100, /*screen_height=*/0,
                               /*native_screen_width=*/200,
                               /*native_screen_height=*/200),
               HasSubstr("Bad ViewPortContext, scale is 0."));
}

TEST(CommonTypesTest, ViewPortContextOk) {
  const ViewPortContext view_port_ctx =
      ViewPortContext(/*screen_width=*/600, /*screen_height=*/1000,
                      /*native_screen_width=*/1200,
                      /*native_screen_height=*/1800);

  EXPECT_FLOAT_EQ(view_port_ctx.scale(), 0.5f);
  EXPECT_FLOAT_EQ(view_port_ctx.offset_x(), 0);
  EXPECT_FLOAT_EQ(view_port_ctx.offset_y(), 50);
  EXPECT_FLOAT_EQ(view_port_ctx.native_screen_width(), 1200);
  EXPECT_FLOAT_EQ(view_port_ctx.native_screen_height(), 1800);
}

TEST(CommonTypesTest, ScreenPositionToPoint) {
  EXPECT_EQ(ScreenPosition(400, 950).ToFPoint(), FPoint(400, 950));
}

TEST(CommonTypesTest, NativeScreenPositionToActualScreenPosition) {
  const ViewPortContext view_port_ctx =
      ViewPortContext(/*screen_width=*/600, /*screen_height=*/1000,
                      /*native_screen_width=*/1200,
                      /*native_screen_height=*/1800);

  EXPECT_EQ(ScreenPosition(400, 950).ToActual(view_port_ctx),
            ScreenPosition(200, 525));
}

TEST(CommonTypesTest, ActualScreenPositionToNativeScreenPositionInLetterBox) {
  const ViewPortContext view_port_ctx =
      ViewPortContext(/*screen_width=*/600, /*screen_height=*/1000,
                      /*native_screen_width=*/1200,
                      /*native_screen_height=*/1800);

  EXPECT_FALSE(ScreenPosition(400, 951).ToNative(view_port_ctx).has_value());
}

TEST(CommonTypesTest, ActualScreenPositionToNativeScreenPosition) {
  const ViewPortContext view_port_ctx =
      ViewPortContext(/*screen_width=*/600, /*screen_height=*/1000,
                      /*native_screen_width=*/1200,
                      /*native_screen_height=*/1800);

  EXPECT_EQ(ScreenPosition(400, 500).ToNative(view_port_ctx).value(),
            ScreenPosition(800, 900));
}

TEST(CommonTypesTest, EqualityScreenPosition) {
  EXPECT_EQ(ScreenPosition(1.3, 2.7), ScreenPosition(1.3, 2.7));
  EXPECT_NE(ScreenPosition(1.2, 2.7), ScreenPosition(1.3, 2.7));
  EXPECT_NE(ScreenPosition(1.1, 2.1), ScreenPosition(1.1, 2.112));
}

TEST(CommonTypesTest, EqualityWorldPosition) {
  EXPECT_EQ(WorldPosition(1.3, 2.7), WorldPosition(1.3, 2.7));
  EXPECT_NE(WorldPosition(1.3, 2.6), WorldPosition(1.3, 2.7));
  EXPECT_NE(WorldPosition(1.3, 2.6), WorldPosition(1.2999, 2.6));
}

TEST(CommonTypesTest, WorldPositionToString) {
  constexpr WorldPosition pos = WorldPosition(1.3, 2.7);

  std::ostringstream oss;
  oss << pos;

  EXPECT_EQ(oss.str(), "WorldPosition (1.3, 2.7)");
}

TEST(CommonTypesTest, WorldPositionToPoint) {
  EXPECT_EQ(WorldPosition(400, 950).ToFPoint(), FPoint(400, 950));
}

TEST(CommonTypesTest, ScreenPositionToString) {
  constexpr ScreenPosition pos = ScreenPosition(1.3, 2.7);

  std::ostringstream oss;
  oss << pos;

  EXPECT_EQ(oss.str(), "ScreenPosition (1.3, 2.7)");
}

TEST(CommonTypesTest, ColorRGBAToString) {
  constexpr ColorRGBA color = {.r = 1, .g = 2, .b = 3, .a = 4};

  std::ostringstream oss;
  oss << color;

  EXPECT_EQ(oss.str(), "ColorRGBA (r: 1, g: 2, b: 3, a: 4)");
}

TEST(CommonTypesTest, FPointToString) {
  constexpr FPoint point = {.x = 1, .y = 2};

  std::ostringstream oss;
  oss << point;

  EXPECT_EQ(oss.str(), "FPoint (x: 1, y: 2)");
}

TEST(CommonTypesTest, FLineToString) {
  constexpr FLine line = {.a = {.x = 1, .y = 2}, .b = {.x = 3, .y = 4}};

  std::ostringstream oss;
  oss << line;

  EXPECT_EQ(oss.str(),
            "FLine (a: FPoint (x: 1, y: 2), b: FPoint (x: 3, y: 4))");
}

TEST(CommonTypesTest, FRectangleToString) {
  constexpr FRectangle rectangle = {
      .top_left = {.x = 1, .y = 2}, .width = 3, .height = 4};

  std::ostringstream oss;
  oss << rectangle;

  EXPECT_EQ(oss.str(),
            "FRectangle (top_left: FPoint (x: 1, y: 2), width: 3, height: 4)");
}

TEST(CommonTypesTest, FCircleToString) {
  constexpr FCircle circle = {.center = {.x = 1, .y = 2}, .radius = 3};

  std::ostringstream oss;
  oss << circle;

  EXPECT_EQ(oss.str(), "FCircle (center: FPoint (x: 1, y: 2), radius: 3)");
}

TEST(CommonTypesTest, PointEquality) {
  EXPECT_EQ(FPoint(1, 2), FPoint(1, 2));
  EXPECT_NE(FPoint(1, 2), FPoint(1, 3));
}

}  // namespace
}  // namespace api
}  // namespace lib
