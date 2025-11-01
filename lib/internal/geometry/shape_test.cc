#include "lib/internal/geometry/shape.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace {

TEST(ShapeTest, PointIsLowerLeft) {
  const Point a{1, 2};
  const Point b{1, 2};

  EXPECT_FALSE(a.IsLowerLeft(b));
  EXPECT_FALSE(b.IsLowerLeft(a));
}

TEST(ShapeTest, PointDistanceZero) {
  const Point a{1, 2};
  const Point b{1, 2};

  EXPECT_DOUBLE_EQ(a.Distance(b), 0);
}

TEST(ShapeTest, PointDistance) {
  const Point a{0, 0};
  const Point b{3, 4};

  EXPECT_DOUBLE_EQ(a.Distance(b), 5);
}

TEST(ShapeTest, PointCenter) {
  const Point a{1, 2};

  EXPECT_DOUBLE_EQ(a.center_x(), 1);
  EXPECT_DOUBLE_EQ(a.center_y(), 2);
}

TEST(ShapeTest, LineCenter) {
  const Line a{Point{1, 2}, Point{1, 4}};

  EXPECT_DOUBLE_EQ(a.center_x(), 1);
  EXPECT_DOUBLE_EQ(a.center_y(), 3);
}

TEST(ShapeTest, LineIsOnLine) {
  const Line a{Point{0, 0}, Point{6, 8}};

  EXPECT_TRUE(a.IsOnLine(Point{3, 4}));
  EXPECT_FALSE(a.IsOnLine(Point{3, 3}));
}

TEST(ShapeTest, LineNotAligned) {
  const Line a{Point{0, 0}, Point{6, 8}};

  EXPECT_EQ(a.axis_aligned, Line::Aligned::NOT);
}

TEST(ShapeTest, LineYAligned) {
  const Line a{Point{1, 4}, Point{1, 8}};

  EXPECT_EQ(a.axis_aligned, Line::Aligned::Y);
}

TEST(ShapeTest, LineXAligned) {
  const Line a{Point{1, 2}, Point{6, 2}};

  EXPECT_EQ(a.axis_aligned, Line::Aligned::X);
}

TEST(ShapeTest, RectangleCenter) {
  const Rectangle a{Point{0, 4}, Point{4, 0}};

  EXPECT_DOUBLE_EQ(a.center_x(), 2);
  EXPECT_DOUBLE_EQ(a.center_y(), 2);
}

}  // namespace
}  // namespace internal
}  // namespace lib
