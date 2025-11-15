#include "lib/api/abilities/ability.h"

#include <list>
#include <optional>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/abilities/controls_mock.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {
namespace {

using objects::MovableObject;
using objects::StaticObject;
using ::testing::HasSubstr;

class MoveAbilityTest : public ::testing::Test {};

class DummyMovableObject : public MovableObject {
 public:
  DummyMovableObject(const double velocity,
                     const std::pair<double, double> hit_box_center)
      : MovableObject(
            /*kind=*/objects::kPlayer,
            MovableObjectOpts{.is_hit_box_active = false,
                              .should_draw_hit_box = false,
                              .attach_camera = false,
                              .velocity = velocity},
            hit_box_center,
            /*hit_box_radius=*/3) {}

  bool OnCollisionCallback(Object& other_object) override { return false; }
};

using MoveAbilityDeathTest = MoveAbilityTest;

TEST(MoveAbilityDeathTest, UserNotMovable) {
  StaticObject static_object = StaticObject(
      /*kind=*/objects::kEnemy, /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  MoveAbility ability =
      MoveAbility(std::make_unique<ControlsMock>(),
                  MoveAbility::MoveAbilityOpts{.key_left = std::nullopt,
                                               .key_right = std::nullopt,
                                               .key_top = std::nullopt,
                                               .key_bottom = std::nullopt});

  ability.set_user(&static_object);

  const Camera camera;
  EXPECT_DEATH(ability.Use(camera),
               HasSubstr("ability user is not of correct type."));
}

TEST(MoveAbilityTest, Move) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  movable_object.SetDirectionGlobal(1, 0);
  MoveAbility ability =
      MoveAbility(std::make_unique<ControlsMock>(),
                  MoveAbility::MoveAbilityOpts{.key_left = std::nullopt,
                                               .key_right = std::nullopt,
                                               .key_top = std::nullopt,
                                               .key_bottom = std::nullopt});
  ability.set_user(&movable_object);

  const Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), 1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

TEST(MoveAbilityTest, MoveLeft) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  MoveAbility ability = MoveAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/true, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      MoveAbility::MoveAbilityOpts{.key_left = kKeyA,
                                   .key_right = std::nullopt,
                                   .key_top = std::nullopt,
                                   .key_bottom = std::nullopt});
  ability.set_user(&movable_object);

  const Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), -1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

TEST(MoveAbilityTest, MoveRight) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  MoveAbility ability = MoveAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/true, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      MoveAbility::MoveAbilityOpts{.key_left = std::nullopt,
                                   .key_right = kKeyA,
                                   .key_top = std::nullopt,
                                   .key_bottom = std::nullopt});
  ability.set_user(&movable_object);

  const Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), 1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

TEST(MoveAbilityTest, MoveTop) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  MoveAbility ability = MoveAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/true, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      MoveAbility::MoveAbilityOpts{.key_left = std::nullopt,
                                   .key_right = std::nullopt,
                                   .key_top = kKeyA,
                                   .key_bottom = std::nullopt});
  ability.set_user(&movable_object);

  const Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), 0);
  EXPECT_EQ(movable_object.direction_y(), -1);
}

TEST(MoveAbilityTest, MoveBottom) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  MoveAbility ability = MoveAbility(
      std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/true, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}),
      MoveAbility::MoveAbilityOpts{.key_left = std::nullopt,
                                   .key_right = std::nullopt,
                                   .key_top = std::nullopt,
                                   .key_bottom = kKeyA});
  ability.set_user(&movable_object);

  const Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), 0);
  EXPECT_EQ(movable_object.direction_y(), 1);
}

}  // namespace
}  // namespace abilities
}  // namespace api
}  // namespace lib
