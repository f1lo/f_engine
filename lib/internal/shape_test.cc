#include "lib/internal/shape.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace {

TEST(ShapeTest, IsLowerLeftEqualPointsReturnFalse) {
  const Point a{1, 2};
  const Point b{1, 2};

  EXPECT_FALSE(a.IsLowerLeft(b));
  EXPECT_FALSE(b.IsLowerLeft(a));
}

} // namespace
} // namespace internal
} // namespace lib
