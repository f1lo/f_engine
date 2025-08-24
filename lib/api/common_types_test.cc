#include "lib/api/common_types.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace api {
namespace {

TEST(CommonTypesTest, TestEqualityScreenPosition) {
  EXPECT_EQ(ScreenPosition(1.3, 2.7), ScreenPosition(1.3, 2.7));
  EXPECT_NE(ScreenPosition(1.2, 2.7), ScreenPosition(1.3, 2.7));
}

TEST(CommonTypesTest, TestEqualityWorldPosition) {
  EXPECT_EQ(WorldPosition(1.3, 2.7), WorldPosition(1.3, 2.7));
  EXPECT_NE(WorldPosition(1.3, 2.6), WorldPosition(1.3, 2.7));
}

}  // namespace
}  // namespace api
}  // namespace lib
