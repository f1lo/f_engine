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

}  // namespace
}  // namespace api
}  // namespace lib
