#include "lib/api/objects/static_object.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/sprites/sprite_factory.h"

namespace lib {
namespace api {
namespace objects {

using sprites::SpriteFactory;

namespace {

constexpr unsigned int kTextureId = 7;
constexpr int kTextureWidth = 300;
constexpr int kTextureHeight = 200;

}  // namespace

class StaticObjectTest : public ::testing::Test {
 public:
  StaticObjectTest()
      : sprite_factory_(
            SpriteFactory(kTextureId, kTextureWidth, kTextureHeight)) {}

 protected:
  SpriteFactory sprite_factory_;
};

namespace {

TEST_F(StaticObjectTest, CircleYBaseWithSprite) {
  StaticObject static_object = StaticObject(
      /*type=*/ObjectTypeFactory::MakeEnemy(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3,
      sprite_factory_.MakeStaticSprite("a/b/picture.png"));

  EXPECT_EQ(static_object.YBase(), kTextureHeight / 2);
}

TEST_F(StaticObjectTest, RectangleYBaseWithSprite) {
  StaticObject static_object = StaticObject(
      /*type=*/ObjectTypeFactory::MakeEnemy(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 2}, {2, 2}, {2, 0}}),
      sprite_factory_.MakeStaticSprite("a/b/picture.png"));

  EXPECT_EQ(static_object.YBase(), kTextureHeight / 2 + 1);
}

TEST_F(StaticObjectTest, OnCollisionCallbackDoesNothing) {
  StaticObject static_object = StaticObject(
      /*type=*/ObjectTypeFactory::MakeEnemy(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 2}, {2, 2}, {2, 0}}),
      sprite_factory_.MakeStaticSprite("a/b/picture.png"));

  EXPECT_FALSE(static_object.OnCollisionCallback(static_object));
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib