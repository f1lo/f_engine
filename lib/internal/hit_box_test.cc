#include "lib/internal/hit_box.h"

#include "absl/status/statusor.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/common_types.h"

namespace lib {
namespace internal {
namespace {

using api::FCircle;
using api::FLine;
using api::FPoint;
using api::FRectangle;
using ::testing::HasSubstr;

class HitBoxTest : public ::testing::Test {
 public:
  HitBoxTest()
      : point_(HitBox::CreateHitBox({FPoint{0, 0}})),
        different_point_(HitBox::CreateHitBox({FPoint{1, 3}})),
        line_(HitBox::CreateHitBox({FPoint{-8.0f, 2.0f}, FPoint{4.0f, 8.0f}})),
        intersecting_line_(
            HitBox::CreateHitBox({FPoint{-2, -6}, FPoint{-2, 6}})),
        parallel_line_(HitBox::CreateHitBox({FPoint{-4, -6}, FPoint{8, 0}})),
        rectangle_(HitBox::CreateHitBox(
            FRectangle{.top_left = {-5, -1}, .width = 11, .height = 5})),
        circle_(HitBox::CreateHitBox(
            FCircle{.center = FPoint{2, 2}, .radius = 3})) {}

 protected:
  HitBox point_;
  HitBox different_point_;
  HitBox line_;
  HitBox intersecting_line_;
  HitBox parallel_line_;
  HitBox rectangle_;
  HitBox circle_;
};

TEST_F(HitBoxTest, PointsCollide) {
  EXPECT_TRUE(point_.CollidesWith(point_));
}

TEST_F(HitBoxTest, PointsDoNotMatch) {
  EXPECT_FALSE(point_.CollidesWith(different_point_));
}

TEST_F(HitBoxTest, PointOnLine) {
  const HitBox point_on_line = HitBox::CreateHitBox({FPoint{0.0f, 6.0f}});

  EXPECT_TRUE(line_.CollidesWith(point_on_line));
  EXPECT_TRUE(point_on_line.CollidesWith(line_));
}

TEST_F(HitBoxTest, PointOnLineVertex) {
  const HitBox point_on_line = HitBox::CreateHitBox({FPoint{4, 8}});

  EXPECT_TRUE(line_.CollidesWith(point_on_line));
  EXPECT_TRUE(point_on_line.CollidesWith(line_));
}

TEST_F(HitBoxTest, PointNotOnLine) {
  const HitBox point = HitBox::CreateHitBox({FPoint{4, 9}});

  EXPECT_FALSE(line_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(line_));
}

TEST_F(HitBoxTest, PointInRectangle) {
  const HitBox point = HitBox::CreateHitBox({FPoint{0, 0}});

  EXPECT_TRUE(rectangle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, PointOnRectangleBorder) {
  const HitBox point = HitBox::CreateHitBox({FPoint{-2, -1}});

  EXPECT_TRUE(rectangle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, PointOutsideRectangle) {
  const HitBox point = HitBox::CreateHitBox({FPoint{0, -5}});

  EXPECT_FALSE(rectangle_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, PointInCircle) {
  const HitBox point = HitBox::CreateHitBox({FPoint{4, 2}});

  EXPECT_TRUE(circle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(circle_));
}

TEST_F(HitBoxTest, PointOnCircleBorder) {
  const HitBox point = HitBox::CreateHitBox({FPoint{5, 2}});

  EXPECT_TRUE(circle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(circle_));
}

TEST_F(HitBoxTest, PointOutsidenCircle) {
  const HitBox point = HitBox::CreateHitBox({FPoint{2, 6}});

  EXPECT_FALSE(circle_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(circle_));
}

TEST_F(HitBoxTest, LinesCross) {
  const HitBox line_a = HitBox::CreateHitBox({FLine{{-8, 2}, {4, 8}}});
  const HitBox line_b = HitBox::CreateHitBox({FLine{{-6, 4}, {-2, 2}}});

  EXPECT_TRUE(line_.CollidesWith(intersecting_line_));
  EXPECT_TRUE(line_a.CollidesWith(line_b));
}

TEST_F(HitBoxTest, LinesParallel) {
  EXPECT_FALSE(line_.CollidesWith(parallel_line_));
}

TEST_F(HitBoxTest, LinesCollidesWithRectangle) {
  const HitBox line = HitBox::CreateHitBox({FLine{{-6, 4}, {-2, 2}}});

  EXPECT_TRUE(line.CollidesWith(rectangle_));
  EXPECT_TRUE(rectangle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineInsideRectangle) {
  const HitBox line = HitBox::CreateHitBox({FLine{{-2, 2}, {2, 0}}});

  EXPECT_TRUE(line.CollidesWith(rectangle_));
  EXPECT_TRUE(rectangle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineOutsideRectangle) {
  const HitBox line = HitBox::CreateHitBox({FLine{{-1, 6}, {-4, 5}}});

  EXPECT_FALSE(line.CollidesWith(rectangle_));
  EXPECT_FALSE(rectangle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineCrossesCircle) {
  const HitBox line = HitBox::CreateHitBox({FLine{{0, -1}, {2, 0}}});

  EXPECT_TRUE(line.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineInsideCircle) {
  const HitBox line = HitBox::CreateHitBox({FLine{{2, 0}, {4, 4}}});

  EXPECT_TRUE(line.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LinePassesThroughCircle) {
  const HitBox line = HitBox::CreateHitBox({FLine{{-5, 4}, {6, 4}}});

  EXPECT_TRUE(line.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineOutsideCircle) {
  const HitBox line = HitBox::CreateHitBox({FLine{{-1, 0}, {2, -2}}});
  const HitBox line_x = HitBox::CreateHitBox({FLine{{0, 6}, {3, 6}}});
  const HitBox line_y = HitBox::CreateHitBox({FLine{{6, 0}, {6, 3}}});

  EXPECT_FALSE(line.CollidesWith(circle_));
  EXPECT_FALSE(line_x.CollidesWith(circle_));
  EXPECT_FALSE(line_y.CollidesWith(circle_));
  EXPECT_FALSE(circle_.CollidesWith(line));
  EXPECT_FALSE(circle_.CollidesWith(line_x));
  EXPECT_FALSE(circle_.CollidesWith(line_y));
}

TEST_F(HitBoxTest, LineTouchesCircle) {
  const HitBox line_x = HitBox::CreateHitBox({FLine{{0, 5}, {3, 5}}});
  const HitBox line_y = HitBox::CreateHitBox({FLine{{5, 0}, {5, 3}}});

  EXPECT_TRUE(circle_.CollidesWith(line_x));
  EXPECT_TRUE(circle_.CollidesWith(line_y));
  EXPECT_TRUE(line_x.CollidesWith(circle_));
  EXPECT_TRUE(line_y.CollidesWith(circle_));
}

TEST_F(HitBoxTest, RectanglesIntersect) {
  const HitBox rectangle = HitBox::CreateHitBox(
      FRectangle{.top_left = {-7, -2}, .width = 3, .height = 3});

  EXPECT_TRUE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, RectanglesDoNotIntersect) {
  const HitBox rectangle = HitBox::CreateHitBox(
      FRectangle{.top_left = {2, 5}, .width = 2, .height = 6});

  EXPECT_FALSE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, RectanglesTouchSinglePoint) {
  const HitBox rectangle = HitBox::CreateHitBox(
      FRectangle{.top_left = {-7, -3}, .width = 2, .height = 2});

  EXPECT_TRUE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, RectanglesTouchSide) {
  const HitBox rectangle = HitBox::CreateHitBox(
      FRectangle{.top_left = {-7, -3}, .width = 4, .height = 2});

  EXPECT_TRUE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, CircleCenterInsideRectangle) {
  EXPECT_TRUE(circle_.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, CircleCenterOutsideRectangleButIntersects) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = FPoint{3, 6}, .radius = 3});
  const HitBox rectangle_touches = HitBox::CreateHitBox(
      FRectangle{.top_left = {2, -3}, .width = 2, .height = 2});
  const HitBox rectangle_intersects = HitBox::CreateHitBox(
      FRectangle{.top_left = {-1, -1}, .width = 1, .height = 1});

  EXPECT_TRUE(rectangle_.CollidesWith(circle));
  EXPECT_TRUE(circle.CollidesWith(rectangle_));
  EXPECT_TRUE(rectangle_touches.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(rectangle_touches));
  EXPECT_TRUE(rectangle_intersects.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(rectangle_intersects));
}

TEST_F(HitBoxTest, CirclesCollide) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = FPoint{4, 5}, .radius = 1});
  const HitBox circle_touches =
      HitBox::CreateHitBox(FCircle{.center = FPoint{7, 2}, .radius = 2});
  const HitBox circle_inside =
      HitBox::CreateHitBox(FCircle{.center = FPoint{4, 2}, .radius = 1});

  EXPECT_TRUE(circle_.CollidesWith(circle));
  EXPECT_TRUE(circle_.CollidesWith(circle_touches));
  EXPECT_TRUE(circle_.CollidesWith(circle_inside));
}

TEST_F(HitBoxTest, CircleLineNoCollision) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = FPoint{10, 3}, .radius = 3});
  const HitBox line = HitBox::CreateHitBox({FLine{{0, 0}, {7, 0}}});

  EXPECT_FALSE(circle.CollidesWith(line));
  EXPECT_FALSE(line.CollidesWith(circle));
}

TEST_F(HitBoxTest, CircleLineNoCollision2) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = {6, 3}, .radius = 2});
  const HitBox line = HitBox::CreateHitBox({FLine{{0, 0}, {7, 0}}});

  EXPECT_FALSE(circle.CollidesWith(line));
  EXPECT_FALSE(line.CollidesWith(circle));
}

TEST_F(HitBoxTest, CirclesDoNotCollide) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = {4, 6}, .radius = 1});
  const HitBox circle_touches =
      HitBox::CreateHitBox(FCircle{.center = {7, 2}, .radius = 1});
  const HitBox circle_inside =
      HitBox::CreateHitBox(FCircle{.center = {1, -2}, .radius = 1});

  EXPECT_FALSE(circle_.CollidesWith(circle));
  EXPECT_FALSE(circle_.CollidesWith(circle_touches));
  EXPECT_FALSE(circle_.CollidesWith(circle_inside));
}

TEST_F(HitBoxTest, CircleRectangleDoesNotCollide) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = {1443, 179}, .radius = 20});
  const HitBox rectangle = HitBox::CreateHitBox(
      FRectangle{.top_left = {1205, 130}, .width = 130, .height = 30});

  EXPECT_FALSE(circle.CollidesWith(rectangle));
  EXPECT_FALSE(rectangle.CollidesWith(circle));
}

TEST_F(HitBoxTest, RectangleReflectionTop) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = {750, 942}, .radius = 20});
  const HitBox rectangle = HitBox::CreateHitBox(
      FRectangle{.top_left = {696, 965}, .width = 300, .height = 25});
  constexpr Vector speed = {10, 20};

  EXPECT_EQ(rectangle.Reflect(circle, speed.x, speed.y),
            std::make_pair(speed.x, -speed.y));
}

TEST_F(HitBoxTest, RectangleReflectionTop2) {
  const HitBox circle =
      HitBox::CreateHitBox(FCircle{.center = {750, 942}, .radius = 20});
  const HitBox rectangle = HitBox::CreateHitBox(
      FRectangle{.top_left = {336, 965}, .width = 300, .height = 25});
  constexpr Vector speed = {10, 20};

  EXPECT_EQ(rectangle.Reflect(circle, speed.x, speed.y),
            std::make_pair(-speed.x, speed.y));
}

}  // namespace
}  // namespace internal
}  // namespace lib
