#include "lib/api/stats.h"

#include "absl/time/time.h"
#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/objects/object_type.h"

namespace lib {
namespace api {

using objects::ObjectType;
using objects::ObjectTypeFactory;

class StatsTest : public ::testing::Test {
 public:
  StatsTest() = default;

 protected:
  Stats stats_;
  ObjectTypeFactory object_type_factory_;
};

TEST_F(StatsTest, NoCollisionAdded) {
  const ObjectType type_1 = object_type_factory_.MakeNewObjectType();
  const ObjectType type_2 = object_type_factory_.MakeNewObjectType();

  EXPECT_EQ(stats_.GetCollisionCount(type_1, type_2), 0);
  EXPECT_FALSE(stats_.GetLastCollisionTime(type_1, type_2).has_value());
}

TEST_F(StatsTest, AddCollisionFirst) {
  const ObjectType type_1 = object_type_factory_.MakeNewObjectType();
  const ObjectType type_2 = object_type_factory_.MakeNewObjectType();

  stats_.AddCollision(type_1, type_2);

  EXPECT_EQ(stats_.GetCollisionCount(type_2, type_1), 1);
}

TEST_F(StatsTest, AddCollisionAlreadyExists) {
  const ObjectType type_1 = object_type_factory_.MakeNewObjectType();
  const ObjectType type_2 = object_type_factory_.MakeNewObjectType();

  stats_.AddCollision(type_2, type_1);
  stats_.AddCollision(type_1, type_2);

  EXPECT_EQ(stats_.GetCollisionCount(type_1, type_2), 2);
}

TEST_F(StatsTest, GetLastCollisionTimeFirst) {
  const ObjectType type_1 = object_type_factory_.MakeNewObjectType();
  const ObjectType type_2 = object_type_factory_.MakeNewObjectType();

  stats_.AddCollision(type_2, type_1);

  ASSERT_TRUE(stats_.GetLastCollisionTime(type_1, type_2).has_value());
  EXPECT_LE(stats_.GetLastCollisionTime(type_1, type_2).value(), absl::Now());
}

TEST_F(StatsTest, GetLastCollisionTimeAlreadyExists) {
  const ObjectType type_1 = object_type_factory_.MakeNewObjectType();
  const ObjectType type_2 = object_type_factory_.MakeNewObjectType();

  stats_.AddCollision(type_2, type_1);
  stats_.AddCollision(type_2, type_1);

  ASSERT_TRUE(stats_.GetLastCollisionTime(type_1, type_2).has_value());
  EXPECT_LE(stats_.GetLastCollisionTime(type_1, type_2).value(), absl::Now());
}

}  // namespace api
}  // namespace lib
