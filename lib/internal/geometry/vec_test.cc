#include "vec.h"

#include <numbers>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace {

TEST(VecTest, SquareWorks) {
  EXPECT_DOUBLE_EQ(Vector(3, 7).Square(), 58);
}

TEST(VecTest, LengthWorks) {
  EXPECT_DOUBLE_EQ(Vector(5, 2).Length(), 5.3851648071345037);
}

TEST(VecTest, IsAxisAlignedOk) {
  EXPECT_TRUE(Vector(0, 2).IsAxisAligned());
  EXPECT_TRUE(Vector(2, 0).IsAxisAligned());
}

TEST(VecTest, NotAxisAligned) {
  EXPECT_FALSE(Vector(3, 2).IsAxisAligned());
  EXPECT_FALSE(Vector(2, 123).IsAxisAligned());
}

TEST(VecTest, MultiplyWorks) {
  EXPECT_EQ(Vector(3, 2).Multiply(2), (Vector{6, 4}));
  EXPECT_EQ(Vector(0, 7).Multiply(3), (Vector{0, 21}));
  EXPECT_EQ(Vector(1, 2).Multiply(7), (Vector{7, 14}));
}

TEST(VecTest, ScalarProductWorks) {
  EXPECT_DOUBLE_EQ(Vector(3, 5).DotProduct(Vector{12, 7}), 71);
  EXPECT_DOUBLE_EQ(Vector(7, 0).DotProduct(Vector{0, 17}), 0);
  EXPECT_DOUBLE_EQ(Vector(7, 0).DotProduct(Vector{1, 1}), 7);
}

TEST(VecTest, ProjectionWorks) {
  constexpr Vector v{8, 16};
  constexpr Vector v_to_project{10, 10};

  EXPECT_EQ(v.Project(v_to_project), (Vector{6, 12}));
}

TEST(VecTest, ProjectionOutside) {
  constexpr Vector v{-2, -4};
  constexpr Vector v_to_project{-20, 0};

  EXPECT_EQ(v.Project(v_to_project), (Vector{-4, -8}));
}

TEST(VecTest, ProjectionSameVectors) {
  constexpr Vector v{2, 7};
  constexpr Vector v_to_project{2, 7};

  EXPECT_EQ(v.Project(v_to_project), (Vector{2, 7}));
}

TEST(VecTest, ProjectionOrthogonalVectors) {
  constexpr Vector v{-4, 4};
  constexpr Vector v_to_project{4, 4};

  EXPECT_EQ(v.Project(v_to_project), (Vector{0, 0}));
}

TEST(VecTest, AngleIsCorrect) {
  constexpr Vector v{2, 4};
  constexpr Vector w{1, 0};
  constexpr Vector opposite{-1, 0};

  EXPECT_DOUBLE_EQ(v.Angle(w), atan(2.0));
  EXPECT_DOUBLE_EQ(w.Angle(v), atan(2.0));
  EXPECT_DOUBLE_EQ(opposite.Angle(v), std::numbers::pi_v<double> - atan(2.0));
  EXPECT_DOUBLE_EQ(v.Angle(opposite), std::numbers::pi_v<double> - atan(2.0));
}

TEST(VecTest, RotateWorks) {
  constexpr Vector v{2, 4};
  const Vector w{sqrt(20), 0};
  const Vector opposite{-sqrt(20), 0};
  const double rotation_angle_clockwise = -atan(2.0);
  const double rotation_angle_counter_clockwise =
      atan(2.0) - std::numbers::pi_v<double>;

  EXPECT_EQ(v.Rotate(rotation_angle_clockwise), w);
  EXPECT_EQ(w.Rotate(-rotation_angle_clockwise), v);
  EXPECT_EQ(v.Rotate(-rotation_angle_counter_clockwise), opposite);
  EXPECT_EQ(opposite.Rotate(rotation_angle_counter_clockwise), v);
}

}  // namespace
}  // namespace internal
}  // namespace lib
