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
  HitBoxTest() :
      point_(HitBoxOrDie(HitBox::CreateHitBox({Point{0, 0}}))),
      different_point_(HitBoxOrDie(HitBox::CreateHitBox({Point{1, 3}}))),
      line_(HitBoxOrDie(HitBox::CreateHitBox({Point{-8, 2}, Point{4, 8}}))),
      intersecting_line_(
          HitBoxOrDie(HitBox::CreateHitBox({Point{-2, -6}, Point{-2, 6}}))),
      parallel_line_(
          HitBoxOrDie(HitBox::CreateHitBox({Point{-4, -6}, Point{8, 0}}))),
      rectangle_(HitBoxOrDie(HitBox::CreateHitBox(
          {Point{-4, -6}, Point{8, 0}, Point{4, 8}, Point{-8, 2}}))),
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
      HitBox::CreateHitBox({{-8, 2}, {4, 8}, {-4, -6}, {8, 0}});

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

TEST_F(HitBoxTest, PointsCollide) {
  EXPECT_TRUE(this->point_.CollidesWith(this->point_));
}

TEST_F(HitBoxTest, PointsDoNotMatch) {
  EXPECT_FALSE(this->point_.CollidesWith(this->different_point_));
}

TEST_F(HitBoxTest, PointOnLine) {
  const HitBox point_on_line = HitBoxOrDie(HitBox::CreateHitBox({{0, 6}}));

  EXPECT_TRUE(this->line_.CollidesWith(point_on_line));
  EXPECT_TRUE(point_on_line.CollidesWith(this->line_));
}

TEST_F(HitBoxTest, PointOnLineVertex) {
  const HitBox point_on_line = HitBoxOrDie(HitBox::CreateHitBox({{4, 8}}));

  EXPECT_TRUE(this->line_.CollidesWith(point_on_line));
  EXPECT_TRUE(point_on_line.CollidesWith(this->line_));
}


TEST_F(HitBoxTest, PointNotOnLine) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{4, 9}}));

  EXPECT_FALSE(this->line_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(this->line_));
}

TEST_F(HitBoxTest, PointInRectangle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{0, 0}}));

  EXPECT_TRUE(this->rectangle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(this->rectangle_));
}

TEST_F(HitBoxTest, PointOnRectangleBorder) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{0, 6}}));

  EXPECT_TRUE(this->rectangle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(this->rectangle_));
}

TEST_F(HitBoxTest, PointOutsideRectangle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{0, -5}}));

  EXPECT_FALSE(this->rectangle_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(this->rectangle_));
}


TEST_F(HitBoxTest, PointInCircle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{4, 2}}));

  EXPECT_TRUE(this->circle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(this->circle_));
}

TEST_F(HitBoxTest, PointOnCircleBorder) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{5, 2}}));

  EXPECT_TRUE(this->circle_.CollidesWith(point));
  EXPECT_TRUE(point.CollidesWith(this->circle_));
}

TEST_F(HitBoxTest, PointOutsidenCircle) {
  const HitBox point = HitBoxOrDie(HitBox::CreateHitBox({{2, 6}}));

  EXPECT_FALSE(this->circle_.CollidesWith(point));
  EXPECT_FALSE(point.CollidesWith(this->circle_));
}

TEST_F(HitBoxTest, LinesCross) {
  EXPECT_TRUE(this->line_.CollidesWith(this->intersecting_line_));
}

TEST_F(HitBoxTest, LinesParallel) {
  EXPECT_FALSE(this->line_.CollidesWith(this->parallel_line_));
}

} // namespace
} // namespace internal
} // namespace lib
