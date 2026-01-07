#include "lib/api/objects/projectile_object.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace objects {
namespace {

TEST(ProjectileObjectTest, OnCollisionCallbackDespawns) {
  StaticObject collided_object =
      StaticObject(ObjectTypeFactory::MakePlayer(),
                   StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                                  .should_draw_hit_box = false},
                   /*hit_box_center=*/std::make_pair(1, 1),
                   /*hit_box_radius=*/2);
  ProjectileObject projectile = ProjectileObject(
      ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(1, 1),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{ObjectTypeFactory::MakePlayer()},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<ObjectType>{}});

  projectile.OnCollisionCallback(collided_object);

  EXPECT_TRUE(projectile.deleted());
}

TEST(ProjectileObjectTest, OnCollisionCallbackBottomScreenDespawns) {
  StaticObject collided_object =
      StaticObject(ObjectTypeFactory::MakeScreenBottom(),
                   StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                                  .should_draw_hit_box = false},
                   std::vector<std::pair<float, float>>{{0, 0}, {10, 0}});
  ProjectileObject projectile = ProjectileObject(
      ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<ObjectType>{}});

  projectile.OnCollisionCallback(collided_object);

  EXPECT_TRUE(projectile.deleted());
}

TEST(ProjectileObjectTest, OnCollisionCallbackTopScreenDespawns) {
  StaticObject collided_object =
      StaticObject(ObjectTypeFactory::MakeScreenTop(),
                   StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                                  .should_draw_hit_box = false},
                   std::vector<std::pair<float, float>>{{0, 0}, {10, 0}});
  ProjectileObject projectile = ProjectileObject(
      ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<ObjectType>{}});

  projectile.OnCollisionCallback(collided_object);

  EXPECT_TRUE(projectile.deleted());
}

TEST(ProjectileObjectTest, OnCollisionCallbackLeftScreenDespawns) {
  StaticObject collided_object =
      StaticObject(ObjectTypeFactory::MakeScreenLeft(),
                   StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                                  .should_draw_hit_box = false},
                   std::vector<std::pair<float, float>>{{0, 0}, {10, 0}});
  ProjectileObject projectile = ProjectileObject(
      ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<ObjectType>{}});

  projectile.OnCollisionCallback(collided_object);

  EXPECT_TRUE(projectile.deleted());
}

TEST(ProjectileObjectTest, OnCollisionCallbackRightScreenDespawns) {
  StaticObject collided_object =
      StaticObject(ObjectTypeFactory::MakeScreenRight(),
                   StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                                  .should_draw_hit_box = false},
                   std::vector<std::pair<float, float>>{{0, 0}, {10, 0}});
  ProjectileObject projectile = ProjectileObject(
      ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<ObjectType>{}});

  projectile.OnCollisionCallback(collided_object);

  EXPECT_TRUE(projectile.deleted());
}

TEST(ProjectileObjectTest, OnCollisionCallbackReflects) {
  StaticObject collided_object =
      StaticObject(ObjectTypeFactory::MakeEnemy(),
                   StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                                  .should_draw_hit_box = false},
                   /*hit_box_vertices=*/
                   std::vector<std::pair<float, float>>{{5, 0}, {5, 10}});
  ProjectileObject projectile = ProjectileObject(
      ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(0, 5),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{ObjectTypeFactory::MakeEnemy()},
          .ignore_these_objects = absl::flat_hash_set<ObjectType>{}});
  projectile.SetDirectionGlobal(/*x=*/1, /*y=*/0);

  projectile.OnCollisionCallback(collided_object);

  EXPECT_FLOAT_EQ(projectile.direction_x(), -1.0f);
  EXPECT_FLOAT_EQ(projectile.direction_y(), 0.0f);
}

TEST(ProjectileObjectTest, OnCollisionCallbackIgnores) {
  StaticObject collided_object =
      StaticObject(ObjectTypeFactory::MakeEnemy(),
                   StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                                  .should_draw_hit_box = false},
                   /*hit_box_vertices=*/
                   std::vector<std::pair<float, float>>{{5, 0}, {5, 10}});
  ProjectileObject projectile = ProjectileObject(
      ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(0, 5),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<ObjectType>{},
          .ignore_these_objects =
              absl::flat_hash_set<ObjectType>{ObjectTypeFactory::MakeEnemy()}});
  projectile.SetDirectionGlobal(/*x=*/1, /*y=*/0);

  projectile.OnCollisionCallback(collided_object);

  EXPECT_FLOAT_EQ(projectile.direction_x(), 1.0f);
  EXPECT_FLOAT_EQ(projectile.direction_y(), 0.0f);
  EXPECT_FALSE(projectile.deleted());
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib
