#include "lib/api/abilities/projectile_ability.h"

#include <list>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/controls_mock.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/projectile_object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {
namespace {

constexpr float kScreenWidth = 500;
constexpr float kScreenHeight = 250;
constexpr float kNativeScreenWidth = 1000;
constexpr float kNativeScreenHeight = 500;

using objects::MovableObject;
using objects::ProjectileObject;
using objects::StaticObject;

class DummyMovableObject : public MovableObject {
 public:
  DummyMovableObject(const float velocity, const FPoint hit_box_center)
      : MovableObject(
            /*type=*/objects::ObjectTypeFactory::MakePlayer(),
            MovableObjectOpts{.is_hit_box_active = true,
                              .should_draw_hit_box = false,
                              .attach_camera = false,
                              .velocity = velocity},
            FCircle{.center = hit_box_center, .radius = 3}) {}

  bool OnCollisionCallback(Object& other_object) override { return false; }
};

TEST(ProjectileAbilityTest, InputNotPressed) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakePlayer(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      FCircle{.center = {0, 0}, .radius = 3});
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_type=*/objects::ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileAbility::ProjectileAbilityOpts{.cooldown_sec = 0},
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = FPoint{5, 0},
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<objects::ObjectType>{}});
  ability.set_user(&static_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kScreenWidth, kScreenHeight, kNativeScreenWidth, kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_TRUE(objects_and_abilities.empty());
}

TEST(ProjectileAbilityTest, OnCooldown) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakePlayer(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      FCircle{.center = {0, 0}, .radius = 3});
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_type=*/objects::ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileAbility::ProjectileAbilityOpts{.cooldown_sec = 0},
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = FPoint{5, 0},
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<objects::ObjectType>{}});
  ability.set_user(&static_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kScreenWidth, kScreenHeight, kNativeScreenWidth, kNativeScreenHeight);
  std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});
  objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_TRUE(objects_and_abilities.empty());
}

TEST(ProjectileAbilityTest, ProjectileSpawned) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakePlayer(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      FCircle{.center = {0, 0}, .radius = 3});
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_type=*/objects::ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileAbility::ProjectileAbilityOpts{.cooldown_sec = 0},
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = FPoint{5, 0},
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<objects::ObjectType>{}});
  ability.set_user(&static_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kScreenWidth, kScreenHeight, kNativeScreenWidth, kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_EQ(objects_and_abilities.size(), 1);
  EXPECT_EQ(objects_and_abilities.begin()->first->center(),
            (WorldPosition{.x = 0, .y = 0}));
}

TEST(ProjectileAbilityTest, MovableObjectProjectileSameDirection) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/FPoint{0, 0});
  movable_object.SetDirectionGlobal(3, 4);
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_type=*/objects::ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileAbility::ProjectileAbilityOpts{.cooldown_sec = 0},
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = FPoint{5, 0},
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<objects::ObjectType>{}});
  ability.set_user(&movable_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kScreenWidth, kScreenHeight, kNativeScreenWidth, kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_EQ(objects_and_abilities.size(), 1);
  ProjectileObject* projectile = dynamic_cast<ProjectileObject*>(
      objects_and_abilities.begin()->first.get());
  ASSERT_TRUE(projectile != nullptr);
  EXPECT_EQ(projectile->center(), (WorldPosition{.x = 0, .y = 0}));
  EXPECT_FLOAT_EQ(projectile->direction_x(), 0.6f);
  EXPECT_FLOAT_EQ(projectile->direction_y(), 0.8f);
}

TEST(ProjectileAbilityTest, CreatedProjectileDespawnsOnWorldBorderTouch) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakePlayer(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      FCircle{.center = {0, 0}, .radius = 3});
  ProjectileAbility ability = ProjectileAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      /*projectile_type=*/objects::ObjectTypeFactory::MakeProjectilePlayer(),
      ProjectileAbility::ProjectileAbilityOpts{.cooldown_sec = 0},
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = false,
          .despawn_outside_screen_area = true,
          .velocity = 1,
          .hit_box_center = FPoint{5, 0},
          .hit_box_radius = 3,
          .despawn_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .reflect_on_colliding_with_these_objects =
              absl::flat_hash_set<objects::ObjectType>{},
          .ignore_these_objects = absl::flat_hash_set<objects::ObjectType>{}});
  ability.set_user(&static_object);
  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kScreenWidth, kScreenHeight, kNativeScreenWidth, kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});
  EXPECT_EQ(objects_and_abilities.size(), 1);
  ProjectileObject* projectile = dynamic_cast<ProjectileObject*>(
      objects_and_abilities.begin()->first.get());
  ASSERT_TRUE(projectile != nullptr);
  EXPECT_EQ(projectile->center(), (WorldPosition{.x = 0, .y = 0}));
  StaticObject world_border = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakeWorldBorder(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      FCircle{.center = {0, 0}, .radius = 3});

  projectile->OnCollisionCallback(world_border);

  EXPECT_TRUE(projectile->deleted());
}

}  // namespace
}  // namespace abilities
}  // namespace api
}  // namespace lib
