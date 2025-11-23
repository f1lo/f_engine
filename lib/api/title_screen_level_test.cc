#include "lib/api/title_screen_level.h"

#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/level.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

using abilities::Ability;
using abilities::MoveAbility;
using objects::ObjectType;
using objects::ObjectTypeFactory;
using objects::StaticObject;
using ::testing::UnorderedElementsAre;

class TitleScreenLevelTest : public ::testing::Test {
 public:
  TitleScreenLevelTest() : object_type_factory_(ObjectTypeFactory()) {}

 protected:
  ObjectTypeFactory object_type_factory_;
};

using TitleScreenLevelDeathTest = TitleScreenLevelTest;

TEST_F(TitleScreenLevelDeathTest, StartButtonNotAdded) {
  TitleScreenLevelBuilder builder;

  ObjectType kExitButton = object_type_factory_.MakeNewObjectType();
  EXPECT_DEATH(
      builder
          .AddExitButton(std::make_unique<StaticObject>(
              /*type=*/kExitButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))
          .Build(),
      "Start button not added.");
}

TEST_F(TitleScreenLevelDeathTest, ExitButtonNotAdded) {
  TitleScreenLevelBuilder builder;

  ObjectType kStartButton = object_type_factory_.MakeNewObjectType();
  EXPECT_DEATH(
      builder
          .AddStartButton(std::make_unique<StaticObject>(
              /*type=*/kStartButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))
          .Build(),
      "Exit button not added.");
}

TEST_F(TitleScreenLevelDeathTest, StartButtonAddedTwice) {
  TitleScreenLevelBuilder builder;

  ObjectType kStartButton = object_type_factory_.MakeNewObjectType();
  EXPECT_DEATH(
      builder
          .AddStartButton(std::make_unique<StaticObject>(
              /*type=*/kStartButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))
          .AddStartButton(std::make_unique<StaticObject>(
              /*type=*/kStartButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))
          .Build(),
      "Cannot add start button more than once.");
}

TEST_F(TitleScreenLevelDeathTest, ExitButtonAddedTwice) {
  TitleScreenLevelBuilder builder;

  ObjectType kExitButton = object_type_factory_.MakeNewObjectType();
  EXPECT_DEATH(
      builder
          .AddExitButton(std::make_unique<StaticObject>(
              /*type=*/kExitButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))
          .AddExitButton(std::make_unique<StaticObject>(
              /*type=*/kExitButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))

          .Build(),
      "Cannot add exit button more than once.");
}

TEST_F(TitleScreenLevelTest, StartAndExitAddedOk) {
  TitleScreenLevelBuilder builder;

  ObjectType kStartButton = object_type_factory_.MakeNewObjectType();
  ObjectType kExitButton = object_type_factory_.MakeNewObjectType();
  const std::unique_ptr<TitleScreenLevel> level =
      builder
          .AddStartButton(std::make_unique<StaticObject>(
              /*type=*/kStartButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))
          .AddExitButton(std::make_unique<StaticObject>(
              /*type=*/kExitButton,
              StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                             .should_draw_hit_box = false},
              /*hit_box_vertices=*/
              std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})))
          .Build();

  ASSERT_EQ(level->objects_.size(), 2);
  auto it = level->objects_.begin();
  EXPECT_EQ((*it)->type(), kStartButton);
  ++it;
  EXPECT_EQ((*it)->type(), kExitButton);
  EXPECT_EQ(level->abilities_.size(), 2);
}

}  // namespace api
}  // namespace lib