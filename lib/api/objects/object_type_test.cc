#include "lib/api/objects/object_type.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace api {
namespace objects {
TEST(ObjectTypeTest, EqualityWorks) {
  ObjectTypeFactory factory = ObjectTypeFactory();
  const ObjectType type = factory.MakeNewObjectType();
  const ObjectType copy_type = type;
  const ObjectType different_type = factory.MakeNewObjectType();

  EXPECT_EQ(type, copy_type);
  EXPECT_NE(type, different_type);
}

TEST(ObjectTypeTest, LessWorks) {
  ObjectTypeFactory factory = ObjectTypeFactory();
  const ObjectType type_1 = factory.MakeNewObjectType();
  const ObjectType type_2 = factory.MakeNewObjectType();

  EXPECT_LT(type_1, type_2);
}

TEST(ObjectTypeTest, FactoryMethodsWork) {
  EXPECT_TRUE(ObjectTypeFactory::MakePlayer().IsPlayer());
  EXPECT_TRUE(ObjectTypeFactory::MakeEnemy().IsEnemy());
  EXPECT_TRUE(ObjectTypeFactory::MakeScreenLeft().IsScreenLeft());
  EXPECT_TRUE(ObjectTypeFactory::MakeScreenRight().IsScreenRight());
  EXPECT_TRUE(ObjectTypeFactory::MakeScreenTop().IsScreenTop());
  EXPECT_TRUE(ObjectTypeFactory::MakeScreenBottom().IsScreenBottom());
  EXPECT_TRUE(ObjectTypeFactory::MakeMousePointer().IsMousePointer());
  EXPECT_TRUE(ObjectTypeFactory::MakeButton().IsButton());
  EXPECT_TRUE(ObjectTypeFactory::MakeProjectilePlayer().IsProjectilePlayer());
  EXPECT_TRUE(ObjectTypeFactory::MakeCoordinate().IsCoordinate());
  EXPECT_TRUE(ObjectTypeFactory::MakeWorldBorder().IsWorldBorder());
  EXPECT_TRUE(ObjectTypeFactory::MakeSpriteBoundingBox().IsSpriteBoundingBox());

  EXPECT_FALSE(ObjectTypeFactory::MakePlayer().IsEnemy());
  EXPECT_FALSE(ObjectTypeFactory::MakeEnemy().IsPlayer());
  EXPECT_FALSE(ObjectTypeFactory::MakeScreenLeft().IsScreenRight());
  EXPECT_FALSE(ObjectTypeFactory::MakeScreenRight().IsScreenLeft());
  EXPECT_FALSE(ObjectTypeFactory::MakeScreenTop().IsScreenBottom());
  EXPECT_FALSE(ObjectTypeFactory::MakeScreenBottom().IsScreenTop());
  EXPECT_FALSE(ObjectTypeFactory::MakeMousePointer().IsButton());
  EXPECT_FALSE(ObjectTypeFactory::MakeButton().IsMousePointer());
  EXPECT_FALSE(ObjectTypeFactory::MakeProjectilePlayer().IsCoordinate());
  EXPECT_FALSE(ObjectTypeFactory::MakeCoordinate().IsProjectilePlayer());
  EXPECT_FALSE(ObjectTypeFactory::MakeWorldBorder().IsSpriteBoundingBox());
  EXPECT_FALSE(ObjectTypeFactory::MakeSpriteBoundingBox().IsWorldBorder());
}

}  // namespace objects
}  // namespace api
}  // namespace lib
