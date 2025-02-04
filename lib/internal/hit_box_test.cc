#include "lib/internal/hit_box.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace {

using ::lib::internal::HitBox;
using ::testing::HasSubstr;

TEST(HitBoxTest, ZeroParametersCreationFails) {
  absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox({});

  EXPECT_FALSE(hit_box.ok());
  EXPECT_THAT(hit_box.status().message(),
              HasSubstr("empty vertex set while constructing a hit box"));
}

TEST(HitBoxTest, OneParameterCreationOk) {
  absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox({{0, 0}});

  EXPECT_TRUE(hit_box.ok());
}

TEST(HitBoxTest, NonConvexHullCreationFails) {
  absl::StatusOr<HitBox> hit_box =
      HitBox::CreateHitBox({{0, 0}, {0, 3}, {3, 3}, {2, 2}, {3, 0}});

  EXPECT_FALSE(hit_box.ok());
  EXPECT_THAT(hit_box.status().message(),
              HasSubstr("some of the hit box vertices have been discarded"));
}

TEST(HitBoxTest, ConvexHullCreationOk) {
  absl::StatusOr<HitBox> hit_box =
      HitBox::CreateHitBox({{0, 3}, {0, 0}, {3, 0}, {4, 2}, {3, 3}});

  EXPECT_TRUE(hit_box.ok());
}

} // namespace
} // namespace internal
} // namespace lib
