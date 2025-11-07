#include "lib/api/abilities/move_with_cursor_ability.h"

#include <list>
#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/abilities/controls_mock.h"
#include "lib/api/camera.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {
namespace {

using objects::MovableObject;
using objects::StaticObject;
using ::testing::HasSubstr;

class MoveWithCursorAbilityTest {};

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
            hit_box_center, /*hit_box_radius=*/3) {}

  bool OnCollisionCallback(Object& other_object) override { return false; }
};

using MoveWithCursorAbilityTestDeathTest = MoveWithCursorAbilityTest;

TEST(MoveWithCursorAbilityDeathTest, NonMovableObject) {
  StaticObject static_object = StaticObject(
      /*kind=*/objects::kEnemy, /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>());

  ability.set_user(&static_object);

  Camera camera;
  EXPECT_DEATH(ability.Use(camera), HasSubstr("User not movable"));
}

TEST(MoveWithCursorAbilityTest, MoveNotPressedDirectionNotChanged) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}));
  movable_object.SetDirectionGlobal(-1, 0);
  ability.set_user(&movable_object);

  Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), -1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

TEST(MoveWithCursorAbilityTest, MoveNotPressedDirectionChanges) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/true,
          /*cursor_pos*/ ScreenPosition{.x = -1, .y = 0}));
  ability.set_user(&movable_object);

  Camera camera;
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use(camera);

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), -1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

}  // namespace
}  // namespace abilities
}  // namespace api
}  // namespace lib
