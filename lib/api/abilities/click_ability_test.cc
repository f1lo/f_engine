#include "lib/api/abilities/click_ability.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/abilities/controls_mock.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {
namespace {

using objects::StaticObject;

constexpr float kNativeScreenWidth = 1000;
constexpr float kNativeScreenHeight = 500;

TEST(ClickAbilityTest, NotClicked) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakeEnemy(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  ClickAbility ability = ClickAbility(std::make_unique<ControlsMock>(
      /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/false,
      /*is_secondary_pressed=*/false,
      /*cursor_pos*/ ScreenPosition{.x = 0, .y = 0}));
  ability.set_user(&static_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const auto& objects_and_abilities = ability.Use(camera);

  EXPECT_FALSE(static_object.clicked());
}

TEST(ClickAbilityTest, ClickedButMissed) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakeEnemy(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  ClickAbility ability = ClickAbility(std::make_unique<ControlsMock>(
      /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
      /*is_secondary_pressed=*/false,
      /*cursor_pos*/ ScreenPosition{.x = 5, .y = 5}));
  ability.set_user(&static_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const auto& objects_and_abilities = ability.Use(camera);

  EXPECT_FALSE(static_object.clicked());
}

TEST(ClickAbilityTest, Clicked) {
  StaticObject static_object = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakeEnemy(), /*options=*/
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_center=*/std::make_pair(0, 0), /*hit_box_radius=*/3);
  ClickAbility ability = ClickAbility(std::make_unique<ControlsMock>(
      /*is_pressed=*/false, /*is_down=*/false, /*is_primary_pressed=*/true,
      /*is_secondary_pressed=*/false,
      /*cursor_pos*/ ScreenPosition{.x = 1, .y = 1}));
  ability.set_user(&static_object);

  const Camera camera(kNativeScreenWidth, kNativeScreenHeight);
  const auto& objects_and_abilities = ability.Use(camera);

  EXPECT_TRUE(static_object.clicked());
}

}  // namespace
}  // namespace abilities
}  // namespace api
}  // namespace lib
