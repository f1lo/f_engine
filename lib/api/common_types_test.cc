#include "lib/api/common_types.h"

#include <sstream>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace api {
namespace {

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
  const WorldPosition pos = WorldPosition(1.3, 2.7);

  std::ostringstream oss;
  oss << pos;

  EXPECT_EQ(oss.str(), "WorldPosition (1.3, 2.7)");
}

TEST(CommonTypesTest, ScreenPositionToString) {
  const ScreenPosition pos = ScreenPosition(1.3, 2.7);

  std::ostringstream oss;
  oss << pos;

  EXPECT_EQ(oss.str(), "ScreenPosition (1.3, 2.7)");
}

}  // namespace
}  // namespace api
}  // namespace lib
