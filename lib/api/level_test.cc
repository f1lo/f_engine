#include "lib/api/level.h"

#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/controls.h"
#include "lib/api/abilities/controls_mock.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

using lib::api::abilities::Ability;
using lib::api::abilities::kKeyA;
using lib::api::abilities::MoveAbility;
using lib::api::objects::kCoordinate;
using lib::api::objects::kEnemy;
using lib::api::objects::Kind;
using lib::api::objects::kPlayer;
using lib::api::objects::kScreenBottom;
using lib::api::objects::kScreenLeft;
using lib::api::objects::kScreenRight;
using lib::api::objects::kScreenTop;
using lib::api::objects::StaticObject;
using ::testing::HasSubstr;
using ::testing::UnorderedElementsAre;

namespace {

class DummyLevel : public Level {
 public:
  explicit DummyLevel(const LevelId id) : Level(id) {}

  LevelId MaybeChangeLevel() const override { return kInvalidLevel; }
};

}  // namespace

class LevelTest : public ::testing::Test {
 public:
 protected:
};

using LevelDeathTest = LevelTest;

TEST_F(LevelDeathTest, ScreenEdgeObjectsAreAddedTwice) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);

  EXPECT_DEATH(dummy_builder.WithScreenObjects().WithScreenObjects(),
               HasSubstr("WithScreenObjects() has already been called."));
}

TEST_F(LevelDeathTest, CoordinateObjectsAreAddedTwice) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);

  EXPECT_DEATH(dummy_builder.WithCoordinates().WithCoordinates(),
               HasSubstr("WithCoordinates() has already been called."));
}

TEST_F(LevelTest, ObjectsAreAdded) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);

  dummy_builder.AddObject(std::make_unique<StaticObject>(
      /*kind=*/kPlayer,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/false,
          /*should_draw_hit_box=*/false),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})));
  dummy_builder.AddObject(std::make_unique<StaticObject>(
      /*kind=*/kEnemy,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/false,
          /*should_draw_hit_box=*/false),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})));
  std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();

  ASSERT_EQ(dummy_level->objects_.size(), 2);
  auto it = dummy_level->objects_.begin();
  EXPECT_EQ((*it)->kind(), kPlayer);
  it++;
  EXPECT_EQ((*it)->kind(), kEnemy);
}

TEST_F(LevelTest, ObjectsAndAbilitiesAreAdded) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  std::list<std::unique_ptr<Ability>> abilities;
  abilities.push_back(std::make_unique<MoveAbility>(
      std::make_unique<abilities::ControlsMock>(),
      MoveAbility::MoveAbilityOpts(Ability::AbilityOpts(/*cooldown_sec=*/0),
                                   /*key_left=*/kKeyA, /*key_right=*/kKeyA,
                                   /*key_top=*/kKeyA, /*key_bottom=*/kKeyA)));

  dummy_builder.AddObjectAndAbilities(
      std::make_unique<StaticObject>(
          /*kind=*/kPlayer,
          StaticObject::StaticObjectOpts(
              /*is_hit_box_active=*/false,
              /*should_draw_hit_box=*/false),
          /*hit_box_vertices=*/
          std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})),
      std::move(abilities));
  std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();

  ASSERT_EQ(dummy_level->objects_.size(), 1);
  ASSERT_EQ(dummy_level->abilities_.size(), 1);
  EXPECT_EQ(dummy_level->objects_.front()->kind(), kPlayer);
}

TEST_F(LevelTest, ScreenEdgeObjectsAreAdded) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);

  std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();

  ASSERT_EQ(dummy_level->objects_.size(), 4);
  ASSERT_EQ(dummy_level->screen_edge_objects_.size(), 4);
  std::vector<Kind> object_kinds;
  std::vector<Kind> screen_edge_kinds;
  for (const auto& object : dummy_level->objects_) {
    object_kinds.push_back(object->kind());
  }
  for (const auto& object : dummy_level->screen_edge_objects_) {
    screen_edge_kinds.push_back(object->kind());
  }

  EXPECT_THAT(object_kinds, UnorderedElementsAre(kScreenLeft, kScreenRight,
                                                 kScreenTop, kScreenBottom));
  EXPECT_THAT(screen_edge_kinds,
              UnorderedElementsAre(kScreenLeft, kScreenRight, kScreenTop,
                                   kScreenBottom));
}

TEST_F(LevelTest, CoordinateObjectsAreAdded) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);

  std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithCoordinates().Build();

  ASSERT_EQ(dummy_level->objects_.size(), 2);
  ASSERT_EQ(dummy_level->coordinate_objects_.size(), 2);
  std::vector<Kind> object_kinds;
  std::vector<Kind> coordinate_kinds;
  for (const auto& object : dummy_level->objects_) {
    object_kinds.push_back(object->kind());
  }
  for (const auto& object : dummy_level->coordinate_objects_) {
    coordinate_kinds.push_back(object->kind());
  }

  EXPECT_THAT(object_kinds, UnorderedElementsAre(kCoordinate, kCoordinate));
  EXPECT_THAT(coordinate_kinds, UnorderedElementsAre(kCoordinate, kCoordinate));
}

}  // namespace api
}  // namespace lib