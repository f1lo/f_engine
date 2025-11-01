#include "lib/api/objects/projectile_object.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace objects {
namespace {

TEST(ProjectileObjectTest, OnCollisionCallbackDespawns) {
  StaticObject collided_object =
      StaticObject(kPlayer,
                   StaticObject::StaticObjectOpts(
                       /*is_hit_box_active=*/true,
                       /*should_draw_hit_box=*/false),
                   /*hit_box_center=*/std::make_pair(1, 1),
                   /*hit_box_radius=*/2);
  ProjectileObject projectile = ProjectileObject(
      kProjectilePlayer,
      ProjectileObject::ProjectileObjectOpts(
          /*should_draw_hit_box=*/false,
          /*despawn_outside_screen_area=*/true, /*velocity=*/1,
          /*hit_box_center=*/std::make_pair(1, 1), /*hit_box_radius=*/3,
          /*despawn_on_colliding_with_these_objects=*/
          absl::flat_hash_set<Kind>{kPlayer},
          /*reflect_on_colliding_with_these_objects=*/
          absl::flat_hash_set<Kind>{},
          /*ignore_these_objects=*/
          absl::flat_hash_set<Kind>{}));

  projectile.OnCollisionCallback(collided_object);

  EXPECT_TRUE(projectile.deleted());
}

TEST(ProjectileObjectTest, OnCollisionCallbackReflects) {
  StaticObject collided_object =
      StaticObject(kEnemy,
                   StaticObject::StaticObjectOpts(
                       /*is_hit_box_active=*/true,
                       /*should_draw_hit_box=*/false),
                   /*hit_box_vertices=*/
                   std::vector<std::pair<double, double>>{{5, 0}, {5, 10}});
  ProjectileObject projectile = ProjectileObject(
      kProjectilePlayer,
      ProjectileObject::ProjectileObjectOpts(
          /*should_draw_hit_box=*/false,
          /*despawn_outside_screen_area=*/true, /*velocity=*/1,
          /*hit_box_center=*/std::make_pair(0, 5), /*hit_box_radius=*/3,
          /*despawn_on_colliding_with_these_objects=*/
          absl::flat_hash_set<Kind>{},
          /*reflect_on_colliding_with_these_objects=*/
          absl::flat_hash_set<Kind>{kEnemy},
          /*ignore_these_objects=*/
          absl::flat_hash_set<Kind>{}));
  projectile.SetDirectionGlobal(/*x=*/1, /*y=*/0);

  projectile.OnCollisionCallback(collided_object);

  EXPECT_DOUBLE_EQ(projectile.direction_x(), -1);
  EXPECT_DOUBLE_EQ(projectile.direction_y(), 0);
}

TEST(ProjectileObjectTest, OnCollisionCallbackIgnores) {
  StaticObject collided_object =
      StaticObject(kEnemy,
                   StaticObject::StaticObjectOpts(
                       /*is_hit_box_active=*/true,
                       /*should_draw_hit_box=*/false),
                   /*hit_box_vertices=*/
                   std::vector<std::pair<double, double>>{{5, 0}, {5, 10}});
  ProjectileObject projectile = ProjectileObject(
      kProjectilePlayer,
      ProjectileObject::ProjectileObjectOpts(
          /*should_draw_hit_box=*/false,
          /*despawn_outside_screen_area=*/true, /*velocity=*/1,
          /*hit_box_center=*/std::make_pair(0, 5), /*hit_box_radius=*/3,
          /*despawn_on_colliding_with_these_objects=*/
          absl::flat_hash_set<Kind>{},
          /*reflect_on_colliding_with_these_objects=*/
          absl::flat_hash_set<Kind>{},
          /*ignore_these_objects=*/
          absl::flat_hash_set<Kind>{kEnemy}));
  projectile.SetDirectionGlobal(/*x=*/1, /*y=*/0);

  projectile.OnCollisionCallback(collided_object);

  EXPECT_DOUBLE_EQ(projectile.direction_x(), 1);
  EXPECT_DOUBLE_EQ(projectile.direction_y(), 0);
  EXPECT_FALSE(projectile.deleted());
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib
