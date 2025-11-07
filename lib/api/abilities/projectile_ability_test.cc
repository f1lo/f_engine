#include "lib/api/abilities/projectile_ability.h"

#include <list>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/abilities/controls_mock.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/projectile_object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {
namespace {

using objects::Kind;
using objects::MovableObject;
using objects::ProjectileObject;
using objects::StaticObject;

class DummyMovableObject : public MovableObject {
 public:
  DummyMovableObject(const double velocity,
                     const std::pair<double, double> hit_box_center)
      : MovableObject(
            /*kind=*/objects::kPlayer,
            MovableObjectOpts{.is_hit_box_active = true,
                              .should_draw_hit_box = false,
                              .attach_camera = false,
                              .velocity = velocity},
            hit_box_center, /*hit_box_radius=*/3) {}

  bool OnCollisionCallback(Object& other_object) override { return false; }
};

TEST(ProjectileAbilityTest, InputNotPressed) {
  StaticObject static_object = StaticObject(
      /*kind=*/objects::kPlayer, /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_kind=*/objects::kProjectilePlayer,
      ProjectileAbility::ProjectileAbilityOpts(/*cooldown_sec=*/0),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .ignore_these_objects = absl::flat_hash_set<Kind>{}});
  ability.set_user(&static_object);

  Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
}

TEST(ProjectileAbilityTest, OnCooldown) {
  StaticObject static_object = StaticObject(
      /*kind=*/objects::kPlayer, /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_kind=*/objects::kProjectilePlayer,
      ProjectileAbility::ProjectileAbilityOpts(/*cooldown_sec=*/0),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .ignore_these_objects = absl::flat_hash_set<Kind>{}});
  ability.set_user(&static_object);

  Camera camera;
  std::list<ObjectAndAbilities> objects_and_abilities = ability.Use(camera);
  objects_and_abilities = ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
}

TEST(ProjectileAbilityTest, ProjectileSpawned) {
  StaticObject static_object = StaticObject(
      /*kind=*/objects::kPlayer, /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_kind=*/objects::kProjectilePlayer,
      ProjectileAbility::ProjectileAbilityOpts(/*cooldown_sec=*/0),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .ignore_these_objects = absl::flat_hash_set<Kind>{}});
  ability.set_user(&static_object);

  Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_EQ(objects_and_abilities.size(), 1);
  EXPECT_EQ(objects_and_abilities.begin()->first->center(),
            (WorldPosition{.x = 0, .y = 0}));
}

TEST(ProjectileAbilityTest, MovableObjectProjectileSameDirection) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  movable_object.SetDirectionGlobal(3, 4);
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_kind=*/objects::kProjectilePlayer,
      ProjectileAbility::ProjectileAbilityOpts(/*cooldown_sec=*/0),
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = std::make_pair(5, 0),
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<Kind>{},
          .ignore_these_objects = absl::flat_hash_set<Kind>{}});
  ability.set_user(&movable_object);

  Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_EQ(objects_and_abilities.size(), 1);
  MovableObject* projectile =
      dynamic_cast<MovableObject*>(objects_and_abilities.begin()->first.get());
  ASSERT_TRUE(projectile != nullptr);
  EXPECT_EQ(projectile->center(), (WorldPosition{.x = 0, .y = 0}));
  EXPECT_DOUBLE_EQ(projectile->direction_x(), 0.6);
  EXPECT_DOUBLE_EQ(projectile->direction_y(), 0.8);
}

}  // namespace
}  // namespace abilities
}  // namespace api
}  // namespace lib
