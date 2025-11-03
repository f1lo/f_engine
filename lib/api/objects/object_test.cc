#include "lib/api/objects/object.h"

#include <list>
#include <memory>
#include <vector>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object_utils.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {
namespace {

using ::testing::HasSubstr;

class DummyObject : public Object {
  using Object::Object;

  void Update(
      const std::list<std::unique_ptr<Object>>& other_objects) override {}
  void Draw() const override {}
  bool OnCollisionCallback(Object& other_object) override { return true; }
};

class ObjectTest : public ::testing::Test {};

using ObjectDeathTest = ObjectTest;

TEST(ObjectDeathTest, ObjectCreationFails) {
  EXPECT_DEATH(
      DummyObject(
          /*kind=*/kEnemy,
          /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
          /*hit_box=*/
          CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
              {2, 2}, {10, 2}, {10, 8}})),
      HasSubstr("HitBox::CreateHitBox() failed"));
}

TEST(ObjectTest, ObjectCreationOk) {
  const DummyObject object = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/1, /*radius=*/3));

  EXPECT_EQ(object.kind(), kPlayer);
  EXPECT_EQ(object.center(), WorldPosition(1.0, 1.0));
  EXPECT_FALSE(object.deleted());
  EXPECT_FALSE(object.clicked());
}

TEST(ObjectTest, Collision) {
  const DummyObject circle = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/1, /*radius=*/3));
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));

  EXPECT_TRUE(circle.CollidesWith(rect));
  EXPECT_TRUE(rect.CollidesWith(circle));
}

TEST(ObjectTest, NoCollision) {
  const DummyObject circle = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/1, /*radius=*/3));
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {5, 2}, {10, 2}, {10, 8}, {5, 8}}));

  EXPECT_FALSE(circle.CollidesWith(rect));
  EXPECT_FALSE(rect.CollidesWith(circle));
}

TEST(ObjectTest, HitBoxDisabledNoCollision) {
  const DummyObject circle = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = false, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/1, /*radius=*/3));
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));

  EXPECT_FALSE(circle.CollidesWith(rect));
  EXPECT_FALSE(rect.CollidesWith(circle));
}

TEST(ObjectTest, ObjectDeletedNoCollision) {
  DummyObject circle = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/1, /*radius=*/3));
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));

  EXPECT_TRUE(circle.CollidesWith(rect));
  EXPECT_TRUE(rect.CollidesWith(circle));
  circle.set_deleted(true);
  EXPECT_FALSE(circle.CollidesWith(rect));
  EXPECT_FALSE(rect.CollidesWith(circle));
}

TEST(ObjectTest, CenterOk) {
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));
  const DummyObject line = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(
          std::vector<std::pair<double, double>>{{0, 0}, {0, 7}}));

  EXPECT_EQ(rect.center(), WorldPosition(6.0, 5.0));
  EXPECT_EQ(line.center(), WorldPosition(0.0, 3.5));
}

TEST(ObjectTest, ReflectDeletedObjectSameDirection) {
  DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));
  const DummyObject circle = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/5, /*radius=*/1));
  rect.set_deleted(true);

  EXPECT_EQ(rect.Reflect(circle, 1, -1), std::make_pair(1.0, -1.0));
}

TEST(ObjectTest, ReflectHitBoxDisabledSameDirection) {
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = false, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));
  const DummyObject circle = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/5, /*radius=*/1));

  EXPECT_EQ(rect.Reflect(circle, 1, -1), std::make_pair(1.0, -1.0));
}

TEST(ObjectTest, ReflectOk) {
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));
  const DummyObject circle = DummyObject(
      /*kind=*/kPlayer,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/CreateCircle(/*x=*/1, /*y=*/5, /*radius=*/1));

  EXPECT_EQ(rect.Reflect(circle, 1, -1), std::make_pair(-1.0, -1.0));
}

TEST(ObjectTest, YBase) {
  const DummyObject rect = DummyObject(
      /*kind=*/kEnemy,
      /*options=*/{.is_hit_box_active = true, .should_draw_hit_box = false},
      /*hit_box=*/
      CreateHitBoxOrDie(std::vector<std::pair<double, double>>{
          {2, 2}, {10, 2}, {10, 8}, {2, 8}}));

  EXPECT_EQ(rect.YBase(), 5);
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib