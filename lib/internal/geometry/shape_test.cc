#include "lib/internal/geometry/shape.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace {

TEST(ShapeTest, PointInternalIsLowerLeft) {
  const PointInternal a{1, 2};
  const PointInternal b{1, 2};

  EXPECT_FALSE(a.IsLowerLeft(b));
  EXPECT_FALSE(b.IsLowerLeft(a));
}

TEST(ShapeTest, PointInternalDistanceZero) {
  const PointInternal a{1, 2};
  const PointInternal b{1, 2};

  EXPECT_DOUBLE_EQ(a.Distance(b), 0);
}

TEST(ShapeTest, PointInternalDistance) {
  const PointInternal a{0, 0};
  const PointInternal b{3, 4};

  EXPECT_DOUBLE_EQ(a.Distance(b), 5);
}

TEST(ShapeTest, PointInternalCenter) {
  const PointInternal a{1, 2};

  EXPECT_DOUBLE_EQ(a.center_x(), 1);
  EXPECT_DOUBLE_EQ(a.center_y(), 2);
}

TEST(ShapeTest, LineCenter) {
  const LineInternal a{PointInternal{1, 2}, PointInternal{1, 4}};

  EXPECT_DOUBLE_EQ(a.center_x(), 1);
  EXPECT_DOUBLE_EQ(a.center_y(), 3);
}

TEST(ShapeTest, LineIsOnLine) {
  const LineInternal a{PointInternal{0, 0}, PointInternal{6, 8}};

  EXPECT_TRUE(a.IsOnLine(PointInternal{3, 4}));
  EXPECT_FALSE(a.IsOnLine(PointInternal{3, 3}));
}

TEST(ShapeTest, LineNotAligned) {
  const LineInternal a{PointInternal{0, 0}, PointInternal{6, 8}};

  EXPECT_EQ(a.axis_aligned, LineInternal::Aligned::NOT);
}

TEST(ShapeTest, LineYAligned) {
  const LineInternal a{PointInternal{1, 4}, PointInternal{1, 8}};

  EXPECT_EQ(a.axis_aligned, LineInternal::Aligned::Y);
}

TEST(ShapeTest, LineXAligned) {
  const LineInternal a{PointInternal{1, 2}, PointInternal{6, 2}};

  EXPECT_EQ(a.axis_aligned, LineInternal::Aligned::X);
}

TEST(ShapeTest, RectangleCenter) {
  const RectangleInternal a{PointInternal{0, 4}, PointInternal{4, 0}};

  EXPECT_DOUBLE_EQ(a.center_x(), 2);
  EXPECT_DOUBLE_EQ(a.center_y(), 2);
}

}  // namespace
}  // namespace internal
}  // namespace lib
