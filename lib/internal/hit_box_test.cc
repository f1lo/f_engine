#include "lib/internal/hit_box.h"

#include "absl/status/statusor.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace {

using ::testing::HasSubstr;

class HitBoxTest : public ::testing::Test {
 public:
  HitBoxTest()
      : point_(HitBoxOrDie(HitBox::CreateHitBox({Point{0, 0}}))),
        different_point_(HitBoxOrDie(HitBox::CreateHitBox({Point{1, 3}}))),
        line_(HitBoxOrDie(HitBox::CreateHitBox({Point{-8, 2}, Point{4, 8}}))),
        intersecting_line_(
            HitBoxOrDie(HitBox::CreateHitBox({Point{-2, -6}, Point{-2, 6}}))),
        parallel_line_(
            HitBoxOrDie(HitBox::CreateHitBox({Point{-4, -6}, Point{8, 0}}))),
        rectangle_(HitBoxOrDie(
            HitBox::CreateHitBox({{-5, 4}, {-5, -1}, {6, -1}, {6, 4}}))),
        circle_(HitBoxOrDie(HitBox::CreateHitBox(Point{2, 2}, 3))) {}

 protected:
  static HitBox HitBoxOrDie(absl::StatusOr<HitBox> hit_box) {
    if (!hit_box.ok()) {
      std::abort();
    }

    return std::move(*hit_box);
  }

  HitBox point_;
  HitBox different_point_;
  HitBox line_;
  HitBox intersecting_line_;
  HitBox parallel_line_;
  HitBox rectangle_;
  HitBox circle_;
};

TEST_F(HitBoxTest, ZeroParametersCreationFails) {
  const absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox({});

  EXPECT_FALSE(hit_box.ok());
  EXPECT_THAT(hit_box.status().message(),
              HasSubstr("empty vertex set while constructing a hit box"));
}

TEST_F(HitBoxTest, OneParameterCreationOk) {
  const absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox({{0, 0}});

  EXPECT_TRUE(hit_box.ok());
}

TEST_F(HitBoxTest, NonConvexHullCreationFails) {
  const absl::StatusOr<HitBox> hit_box =
      HitBox::CreateHitBox({{0, 0}, {0, 3}, {3, 3}, {2, 2}});

  EXPECT_FALSE(hit_box.ok());
  EXPECT_THAT(hit_box.status().message(),
              HasSubstr("some of the hit box vertices have been discarded"));
}

TEST_F(HitBoxTest, ConvexHullCreationOk) {
  const absl::StatusOr<HitBox> hit_box =
      HitBox::CreateHitBox({{0, 3}, {0, 0}, {3, 0}, {3, 3}});

  EXPECT_TRUE(hit_box.ok());
}

TEST_F(HitBoxTest, RectangleHullCreationOk) {
  const absl::StatusOr<HitBox> hit_box =
      HitBox::CreateHitBox({{0, 0}, {1, 1}, {0, 1}, {1, 0}});

  EXPECT_TRUE(hit_box.ok());
}

TEST_F(HitBoxTest, PolygonUnimplemented) {
  const absl::StatusOr<HitBox> hit_box =
      HitBox::CreateHitBox({{0, 3}, {0, 0}, {3, 0}, {4, 1}, {1, 4}});

  EXPECT_EQ(hit_box.status().code(), absl::StatusCode::kUnimplemented);
}

TEST_F(HitBoxTest, NonRectangleUnimplemented) {
  const absl::StatusOr<HitBox> hit_box =
      HitBox::CreateHitBox({{0, 3}, {0, 0}, {3, 0}, {4, 1}});

  EXPECT_EQ(hit_box.status().code(), absl::StatusCode::kUnimplemented);
}

TEST_F(HitBoxTest, RectangleNotAligned) {
  const absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox(
      {Point{-4, -6}, Point{8, 0}, Point{4, 8}, Point{-8, 2}});

  EXPECT_EQ(hit_box.status().code(), absl::StatusCode::kUnimplemented);
}

TEST_F(HitBoxTest, PointsCollide) {
  EXPECT_TRUE(point_.CollidesWith(point_));
}

TEST_F(HitBoxTest, PointsDoNotMatch) {
  EXPECT_FALSE(point_.CollidesWith(different_point_));
}

TEST_F(HitBoxTest, PointOnLine) {
  const HitBox point_on_line = HitBoxOrDie(HitBox::CreateHitBox({{0, 6}}));

  EXPECT_TRUE(line_.CollidesWith(point_on_line));
  EXPECT_TRUE(point_on_line.CollidesWith(line_));
}

TEST_F(HitBoxTest, PointOnLineVertex) {
  const HitBox point_on_line = HitBoxOrDie(HitBox::CreateHitBox({{4, 8}}));

  EXPECT_TRUE(line_.CollidesWith(point_on_line));
  EXPECT_TRUE(point_on_line.CollidesWith(line_));
}

TEST_F(HitBoxTest, PointNotOnLine) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{4, 9}}));

  EXPECT_FALSE(line_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(line_));
}

TEST_F(HitBoxTest, PointInRectangle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{0, 0}}));

  EXPECT_TRUE(rectangle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, PointOnRectangleBorder) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{-2, -1}}));

  EXPECT_TRUE(rectangle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, PointOutsideRectangle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{0, -5}}));

  EXPECT_FALSE(rectangle_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, PointInCircle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{4, 2}}));

  EXPECT_TRUE(circle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(circle_));
}

TEST_F(HitBoxTest, PointOnCircleBorder) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{5, 2}}));

  EXPECT_TRUE(circle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(circle_));
}

TEST_F(HitBoxTest, PointOutsidenCircle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{2, 6}}));

  EXPECT_FALSE(circle_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(circle_));
}

TEST_F(HitBoxTest, LinesCross) {
  const HitBox line_a = HitBoxOrDie(HitBox::CreateHitBox({{-8, 2}, {4, 8}}));
  const HitBox line_b = HitBoxOrDie(HitBox::CreateHitBox({{-6, 4}, {-2, 2}}));

  EXPECT_TRUE(line_.CollidesWith(intersecting_line_));
  EXPECT_TRUE(line_a.CollidesWith(line_b));
}

TEST_F(HitBoxTest, LinesParallel) {
  EXPECT_FALSE(line_.CollidesWith(parallel_line_));
}

TEST_F(HitBoxTest, LinesCollidesWithRectangle) {
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{-6, 4}, {-2, 2}}));

  EXPECT_TRUE(line.CollidesWith(rectangle_));
  EXPECT_TRUE(rectangle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineInsideRectangle) {
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{-2, 2}, {2, 0}}));

  EXPECT_TRUE(line.CollidesWith(rectangle_));
  EXPECT_TRUE(rectangle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineOutsideRectangle) {
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{-1, 6}, {-4, 5}}));

  EXPECT_FALSE(line.CollidesWith(rectangle_));
  EXPECT_FALSE(rectangle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineCrossesCircle) {
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{0, -1}, {2, 0}}));

  EXPECT_TRUE(line.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineInsideCircle) {
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{2, 0}, {4, 4}}));

  EXPECT_TRUE(line.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LinePassesThroughCircle) {
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{-5, 4}, {6, 4}}));

  EXPECT_TRUE(line.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(line));
}

TEST_F(HitBoxTest, LineOutsideCircle) {
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{-1, 0}, {2, -2}}));
  const HitBox line_x = HitBoxOrDie(HitBox::CreateHitBox({{0, 6}, {3, 6}}));
  const HitBox line_y = HitBoxOrDie(HitBox::CreateHitBox({{6, 0}, {6, 3}}));

  EXPECT_FALSE(line.CollidesWith(circle_));
  EXPECT_FALSE(line_x.CollidesWith(circle_));
  EXPECT_FALSE(line_y.CollidesWith(circle_));
  EXPECT_FALSE(circle_.CollidesWith(line));
  EXPECT_FALSE(circle_.CollidesWith(line_x));
  EXPECT_FALSE(circle_.CollidesWith(line_y));
}

TEST_F(HitBoxTest, LineTouchesCircle) {
  const HitBox line_x = HitBoxOrDie(HitBox::CreateHitBox({{0, 5}, {3, 5}}));
  const HitBox line_y = HitBoxOrDie(HitBox::CreateHitBox({{5, 0}, {5, 3}}));

  EXPECT_TRUE(circle_.CollidesWith(line_x));
  EXPECT_TRUE(circle_.CollidesWith(line_y));
  EXPECT_TRUE(line_x.CollidesWith(circle_));
  EXPECT_TRUE(line_y.CollidesWith(circle_));
}

TEST_F(HitBoxTest, RectanglesIntersect) {
  const HitBox rectangle =
      HitBoxOrDie(HitBox::CreateHitBox({{-4, 1}, {-7, 1}, {-4, -2}, {-7, -2}}));

  EXPECT_TRUE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, RectanglesDoNotIntersect) {
  const HitBox rectangle =
      HitBoxOrDie(HitBox::CreateHitBox({{2, 5}, {4, 5}, {2, 11}, {4, 11}}));

  EXPECT_FALSE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, RectanglesTouchSinglePoint) {
  const HitBox rectangle = HitBoxOrDie(
      HitBox::CreateHitBox({{-7, -3}, {-7, -1}, {-5, -1}, {-5, -3}}));

  EXPECT_TRUE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, RectanglesTouchSide) {
  const HitBox rectangle = HitBoxOrDie(
      HitBox::CreateHitBox({{-7, -3}, {-7, -1}, {-3, -1}, {-3, -3}}));

  EXPECT_TRUE(rectangle_.CollidesWith(rectangle));
}

TEST_F(HitBoxTest, CircleCenterInsideRectangle) {
  EXPECT_TRUE(circle_.CollidesWith(rectangle_));
}

TEST_F(HitBoxTest, CircleCenterOutsideRectangleButIntersects) {
  const HitBox circle = HitBox::CreateHitBox({3, 6}, 3);
  const HitBox rectangle_touches =
      HitBoxOrDie(HitBox::CreateHitBox({{2, -1}, {4, -3}, {4, -1}, {2, -3}}));
  const HitBox rectangle_intersects =
      HitBoxOrDie(HitBox::CreateHitBox({{-1, -1}, {-1, 0}, {0, -1}, {0, 0}}));

  EXPECT_TRUE(rectangle_.CollidesWith(circle));
  EXPECT_TRUE(circle.CollidesWith(rectangle_));
  EXPECT_TRUE(rectangle_touches.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(rectangle_touches));
  EXPECT_TRUE(rectangle_intersects.CollidesWith(circle_));
  EXPECT_TRUE(circle_.CollidesWith(rectangle_intersects));
}

TEST_F(HitBoxTest, CirclesCollide) {
  const HitBox circle = HitBox::CreateHitBox({4, 5}, 1);
  const HitBox circle_touches = HitBox::CreateHitBox({7, 2}, 2);
  const HitBox circle_inside = HitBox::CreateHitBox({4, 2}, 1);

  EXPECT_TRUE(circle_.CollidesWith(circle));
  EXPECT_TRUE(circle_.CollidesWith(circle_touches));
  EXPECT_TRUE(circle_.CollidesWith(circle_inside));
}

TEST_F(HitBoxTest, CircleLineNoCollision) {
  const HitBox circle = HitBox::CreateHitBox({10, 3}, 3);
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{0, 0}, {7, 0}}));

  EXPECT_FALSE(circle.CollidesWith(line));
  EXPECT_FALSE(line.CollidesWith(circle));
}

TEST_F(HitBoxTest, CircleLineNoCollision2) {
  const HitBox circle = HitBox::CreateHitBox({6, 3}, 2);
  const HitBox line = HitBoxOrDie(HitBox::CreateHitBox({{0, 0}, {7, 0}}));

  EXPECT_FALSE(circle.CollidesWith(line));
  EXPECT_FALSE(line.CollidesWith(circle));
}

TEST_F(HitBoxTest, CirclesDoNotCollide) {
  const HitBox circle = HitBox::CreateHitBox({4, 6}, 1);
  const HitBox circle_touches = HitBox::CreateHitBox({7, 2}, 1);
  const HitBox circle_inside = HitBox::CreateHitBox({1, -2}, 1);

  EXPECT_FALSE(circle_.CollidesWith(circle));
  EXPECT_FALSE(circle_.CollidesWith(circle_touches));
  EXPECT_FALSE(circle_.CollidesWith(circle_inside));
}

TEST_F(HitBoxTest, CircleRectangleDoesNotCollide) {
  const HitBox circle = HitBox::CreateHitBox({1443, 179}, 20);
  const HitBox rectangle = HitBoxOrDie(HitBox::CreateHitBox(
      {{1205, 160}, {1205, 130}, {1335, 130}, {1335, 160}}));

  EXPECT_FALSE(circle.CollidesWith(rectangle));
  EXPECT_FALSE(rectangle.CollidesWith(circle));
}

TEST_F(HitBoxTest, RectangleReflectionTop) {
  const HitBox circle = HitBox::CreateHitBox({750, 942}, 20);
  const HitBox rectangle = HitBoxOrDie(
      HitBox::CreateHitBox({{696, 990}, {696, 965}, {996, 965}, {996, 990}}));
  const Vector speed = {10, 20};

  EXPECT_EQ(rectangle.Reflect(circle, speed.x, speed.y),
            std::make_pair(speed.x, -speed.y));
}

TEST_F(HitBoxTest, RectangleReflectionTop2) {
  const HitBox circle = HitBox::CreateHitBox({750, 942}, 20);
  const HitBox rectangle = HitBoxOrDie(
      HitBox::CreateHitBox({{336, 990}, {336, 965}, {636, 965}, {636, 990}}));
  const Vector speed = {10, 20};

  EXPECT_EQ(rectangle.Reflect(circle, speed.x, speed.y),
            std::make_pair(-speed.x, speed.y));
}

}  // namespace
}  // namespace internal
}  // namespace lib
