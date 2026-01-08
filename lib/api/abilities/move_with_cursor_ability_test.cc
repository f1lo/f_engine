#include "lib/api/abilities/move_with_cursor_ability.h"

#include <list>
#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/controls_mock.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {
namespace {

using objects::MovableObject;
using objects::StaticObject;
using ::testing::HasSubstr;

constexpr float kScreenWidth = 600;
constexpr float kScreenHeight = 300;
constexpr float kNativeScreenWidth = 1500;
constexpr float kNativeScreenHeight = 900;

class MoveWithCursorAbilityTest {};

class DummyMovableObject : public MovableObject {
 public:
  DummyMovableObject(const float velocity, const FPoint hit_box_center)
      : MovableObject(
            /*type=*/objects::ObjectTypeFactory::MakePlayer(),
            MovableObjectOpts{.is_hit_box_active = false,
                              .should_draw_hit_box = false,
                              .attach_camera = false,
                              .velocity = velocity},
            FCircle{.center = hit_box_center, .radius = 3}) {}

  bool OnCollisionCallback(Object& other_object) override { return false; }
};

using MoveWithCursorAbilityTestDeathTest = MoveWithCursorAbilityTest;

TEST(MoveWithCursorAbilityDeathTest, NonMovableObject) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakeEnemy(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                     .should_draw_hit_box = false},
      FCircle{.center = FPoint{0, 0}, .radius = 3});
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>());

  ability.set_user(&static_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kNativeScreenWidth, kNativeScreenHeight, kNativeScreenWidth,
      kNativeScreenHeight);
  EXPECT_DEATH(
      ability.Use({.camera = camera, .view_port_ctx = view_port_context}),
      HasSubstr("User not movable"));
}

TEST(MoveWithCursorAbilityTest, MoveNotPressedDirectionNotChanged) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/FPoint{0, 0});
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/false,
          /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}));
  movable_object.SetDirectionGlobal(-1, 0);
  ability.set_user(&movable_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kNativeScreenWidth, kNativeScreenHeight, kNativeScreenWidth,
      kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), -1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

TEST(MoveWithCursorAbilityTest, MovePressedDirectionChanges) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/FPoint{0, 0});
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/true,
          /*cursor_pos*/ ScreenPosition{.x = 1, .y = 0}));
  ability.set_user(&movable_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kNativeScreenWidth, kNativeScreenHeight, kNativeScreenWidth,
      kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), 1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

TEST(MoveWithCursorAbilityTest,
     MovePressedDirectionChangesDifferentScreenResolution) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/FPoint{0, 0});
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/true,
          /*cursor_pos*/ ScreenPosition{.x = 300, .y = 0}));
  ability.set_user(&movable_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kScreenWidth, kScreenHeight, kNativeScreenWidth, kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), 1);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

TEST(MoveWithCursorAbilityTest, MovePressedOutsideScreenDirectionSame) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/FPoint{0, 0});
  MoveWithCursorAbility ability =
      MoveWithCursorAbility(std::make_unique<ControlsMock>(
          /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
          /*is_secondary_pressed=*/true,
          /*cursor_pos*/ ScreenPosition{.x = 25, .y = 100}));
  ability.set_user(&movable_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const ViewPortContext view_port_context(
      kScreenWidth, kScreenHeight, kNativeScreenWidth, kNativeScreenHeight);
  const std::list<ObjectAndAbilities> objects_and_abilities =
      ability.Use({.camera = camera, .view_port_ctx = view_port_context});

  EXPECT_TRUE(objects_and_abilities.empty());
  EXPECT_EQ(movable_object.direction_x(), 0);
  EXPECT_EQ(movable_object.direction_y(), 0);
}

}  // namespace
}  // namespace abilities
}  // namespace api
}  // namespace lib
