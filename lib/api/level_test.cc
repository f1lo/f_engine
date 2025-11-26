#include "lib/api/level.h"

#include <filesystem>
#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/controls.h"
#include "lib/api/abilities/controls_mock.h"
#include "lib/api/graphics_mock.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/screen_edge_object.h"
#include "lib/api/objects/static_object.h"
#include "lib/api/sprites/sprite_factory.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {

using abilities::Ability;
using abilities::kKeyA;
using abilities::MoveAbility;
using objects::ObjectType;
using objects::ObjectTypeFactory;
using objects::StaticObject;
using sprites::SpriteFactory;
using sprites::SpriteInstance;
using ::testing::ElementsAre;
using ::testing::HasSubstr;

namespace {

constexpr unsigned int kTextureId = 7;
constexpr int kTextureWidth = 30;
constexpr int kTextureHeight = 20;

class DummyLevel : public Level {
 public:
  explicit DummyLevel(const LevelId id) : Level(id) {}

  [[nodiscard]] LevelId MaybeChangeLevel() const override {
    return kInvalidLevel;
  }
};

}  // namespace

class LevelTest : public ::testing::Test {
 public:
  LevelTest()
      : sprite_factory_(
            SpriteFactory(kTextureId, kTextureWidth, kTextureHeight)) {}

  std::unique_ptr<SpriteInstance> MakeSprite() {
    return sprite_factory_.MakeStaticSprite("a/b/picture.png");
  }

 protected:
  SpriteFactory sprite_factory_;
};

using LevelDeathTest = LevelTest;

TEST_F(LevelDeathTest, CleanUpOrDieOutOfSync) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);

  dummy_builder.AddObject(std::make_unique<StaticObject>(
      /*type=*/ObjectTypeFactory::MakePlayer(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})));
  const std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();
  dummy_level->abilities_.clear();

  EXPECT_DEATH(dummy_level->CleanUpOrDie(),
               HasSubstr("Abilities and Objects are out of sync, abilities "
                         "size: 0, objects size: 1"));
}

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

TEST_F(LevelTest, CleanupOrDie) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  std::unique_ptr<StaticObject> static_object = std::make_unique<StaticObject>(
      /*type=*/ObjectTypeFactory::MakePlayer(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 1}}));
  StaticObject* static_object_raw = static_object.get();
  std::list<std::unique_ptr<Ability>> abilities;
  abilities.push_back(std::make_unique<MoveAbility>(
      std::make_unique<abilities::ControlsMock>(),
      MoveAbility::MoveAbilityOpts{.key_left = kKeyA,
                                   .key_right = kKeyA,
                                   .key_top = kKeyA,
                                   .key_bottom = kKeyA}));
  dummy_builder.AddObjectAndAbilities(std::move(static_object),
                                      std::move(abilities));
  const std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();
  static_object_raw->set_deleted(true);

  dummy_level->CleanUpOrDie();

  ASSERT_EQ(dummy_level->objects_.size(), 0);
  ASSERT_EQ(dummy_level->abilities_.size(), 0);
}

TEST_F(LevelTest, ObjectsAreAdded) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);

  dummy_builder.AddObject(std::make_unique<StaticObject>(
      /*type=*/ObjectTypeFactory::MakePlayer(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})));
  dummy_builder.AddObject(std::make_unique<StaticObject>(
      /*type=*/ObjectTypeFactory::MakeEnemy(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})));
  const std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();

  ASSERT_EQ(dummy_level->objects_.size(), 2);
  auto it = dummy_level->objects_.begin();
  EXPECT_TRUE((*it)->type().IsPlayer());
  ++it;
  EXPECT_TRUE((*it)->type().IsEnemy());
}

TEST_F(LevelTest, ObjectsAndAbilitiesAreAdded) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  std::list<std::unique_ptr<Ability>> abilities;
  abilities.push_back(std::make_unique<MoveAbility>(
      std::make_unique<abilities::ControlsMock>(),
      MoveAbility::MoveAbilityOpts{.key_left = kKeyA,
                                   .key_right = kKeyA,
                                   .key_top = kKeyA,
                                   .key_bottom = kKeyA}));

  dummy_builder.AddObjectAndAbilities(
      std::make_unique<StaticObject>(
          /*type=*/ObjectTypeFactory::MakePlayer(),
          StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                         .should_draw_hit_box = false},
          /*hit_box_vertices=*/
          std::vector<std::pair<double, double>>({{0, 0}, {0, 1}})),
      std::move(abilities));
  const std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();

  ASSERT_EQ(dummy_level->objects_.size(), 1);
  ASSERT_EQ(dummy_level->abilities_.size(), 1);
  EXPECT_TRUE(dummy_level->objects_.front()->type().IsPlayer());
}

TEST_F(LevelTest, ScreenEdgeObjects) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();

  ASSERT_EQ(dummy_level->objects_.size(), 4);
  ASSERT_EQ(dummy_level->screen_edge_objects_.size(), 4);
  std::vector<ObjectType> object_types;
  std::vector<ObjectType> screen_edge_types;
  for (const auto& object : dummy_level->objects_) {
    object_types.push_back(object->type());
  }
  for (const auto& object : dummy_level->screen_edge_objects_) {
    screen_edge_types.push_back(object->type());
  }
  dummy_level->UpdateScreenEdges();

  EXPECT_THAT(object_types, ElementsAre(ObjectTypeFactory::MakeScreenLeft(),
                                        ObjectTypeFactory::MakeScreenRight(),
                                        ObjectTypeFactory::MakeScreenTop(),
                                        ObjectTypeFactory::MakeScreenBottom()));
  EXPECT_THAT(screen_edge_types,
              ElementsAre(ObjectTypeFactory::MakeScreenLeft(),
                          ObjectTypeFactory::MakeScreenRight(),
                          ObjectTypeFactory::MakeScreenTop(),
                          ObjectTypeFactory::MakeScreenBottom()));
}

TEST_F(LevelTest, CoordinateObjects) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithCoordinates().Build();

  ASSERT_EQ(dummy_level->objects_.size(), 2);
  ASSERT_EQ(dummy_level->coordinate_objects_.size(), 2);
  std::vector<ObjectType> object_types;
  std::vector<ObjectType> coordinate_types;
  for (const auto& object : dummy_level->objects_) {
    object_types.push_back(object->type());
  }
  for (const auto& object : dummy_level->coordinate_objects_) {
    coordinate_types.push_back(object->type());
  }
  dummy_level->UpdateCoordinateAxes();

  EXPECT_THAT(object_types, ElementsAre(ObjectTypeFactory::MakeCoordinate(),
                                        ObjectTypeFactory::MakeCoordinate()));
  EXPECT_THAT(coordinate_types,
              ElementsAre(ObjectTypeFactory::MakeCoordinate(),
                          ObjectTypeFactory::MakeCoordinate()));
}

TEST_F(LevelTest, WorldBorderObjects) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithWorldBorderX(/*x=*/1000)
          .WithWorldBorderY(/*y=*/200)
          .Build();

  ASSERT_EQ(dummy_level->objects_.size(), 2);
  ASSERT_EQ(dummy_level->world_border_objects_.size(), 2);
  auto it = dummy_level->world_border_objects_.begin();
  EXPECT_TRUE((*it)->type().IsWorldBorder());
  EXPECT_THAT((*it)->center().x, 1000);
  ++it;
  EXPECT_TRUE((*it)->type().IsWorldBorder());
  EXPECT_THAT((*it)->center().y, 200);
}

TEST_F(LevelTest, DrawsNoScreenEdgeObjects) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();
  const StaticObject object = StaticObject(
      /*type=*/ObjectTypeFactory::MakePlayer(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{-1, 50}, {-1, 49}, {1, 50}, {1, 49}}));

  EXPECT_TRUE(dummy_level->ShouldDraw(object));
}

TEST_F(LevelTest, DrawsFullyInsideScreenOnlyHitBox) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();
  dummy_level->screen_edge_objects_.clear();
  constexpr double screen_width = 200;
  constexpr double screen_height = 100;
  const std::unique_ptr<objects::ScreenEdgeObject> screen_left =
      objects::ScreenEdgeObject::MakeLeft(screen_height,
                                          /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_right =
      objects::ScreenEdgeObject::MakeRight(screen_width, screen_height,
                                           /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_top =
      objects::ScreenEdgeObject::MakeTop(screen_width,
                                         /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
      objects::ScreenEdgeObject::MakeBottom(screen_width, screen_height,
                                            /*should_draw_hit_box=*/false);
  dummy_level->screen_edge_objects_.push_back(screen_left.get());
  dummy_level->screen_edge_objects_.push_back(screen_right.get());
  dummy_level->screen_edge_objects_.push_back(screen_top.get());
  dummy_level->screen_edge_objects_.push_back(screen_bottom.get());
  const StaticObject object = StaticObject(
      /*type=*/ObjectTypeFactory::MakePlayer(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{5, 50}, {5, 49}, {90, 50}, {90, 49}}));

  EXPECT_TRUE(dummy_level->ShouldDraw(object));
}

TEST_F(LevelTest, DrawsFullyInsideScreen) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();
  dummy_level->screen_edge_objects_.clear();
  constexpr double screen_width = 200;
  constexpr double screen_height = 100;
  const std::unique_ptr<objects::ScreenEdgeObject> screen_left =
      objects::ScreenEdgeObject::MakeLeft(screen_height,
                                          /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_right =
      objects::ScreenEdgeObject::MakeRight(screen_width, screen_height,
                                           /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_top =
      objects::ScreenEdgeObject::MakeTop(screen_width,
                                         /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
      objects::ScreenEdgeObject::MakeBottom(screen_width, screen_height,
                                            /*should_draw_hit_box=*/false);
  dummy_level->screen_edge_objects_.push_back(screen_left.get());
  dummy_level->screen_edge_objects_.push_back(screen_right.get());
  dummy_level->screen_edge_objects_.push_back(screen_top.get());
  dummy_level->screen_edge_objects_.push_back(screen_bottom.get());
  const StaticObject object = StaticObject(
      /*type=*/ObjectTypeFactory::MakePlayer(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{5, 50}, {5, 49}, {90, 50}, {90, 49}}),
      MakeSprite());

  EXPECT_TRUE(dummy_level->ShouldDraw(object));
}

TEST_F(LevelTest, DrawsPartiallyOutsideScreenOnlyHitBox) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();
  dummy_level->screen_edge_objects_.clear();
  constexpr double screen_width = 200;
  constexpr double screen_height = 100;
  const std::unique_ptr<objects::ScreenEdgeObject> screen_left =
      objects::ScreenEdgeObject::MakeLeft(screen_height,
                                          /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_right =
      objects::ScreenEdgeObject::MakeRight(screen_width, screen_height,
                                           /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_top =
      objects::ScreenEdgeObject::MakeTop(screen_width,
                                         /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
      objects::ScreenEdgeObject::MakeBottom(screen_width, screen_height,
                                            /*should_draw_hit_box=*/false);
  dummy_level->screen_edge_objects_.push_back(screen_left.get());
  dummy_level->screen_edge_objects_.push_back(screen_right.get());
  dummy_level->screen_edge_objects_.push_back(screen_top.get());
  dummy_level->screen_edge_objects_.push_back(screen_bottom.get());

  {
    const StaticObject object_left = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{-1, 50}, {-1, 49}, {1, 50}, {1, 49}}));
    EXPECT_TRUE(dummy_level->ShouldDraw(object_left));
  }

  {
    const StaticObject object_right = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{190, 50}, {190, 49}, {400, 50}, {400, 49}}));

    EXPECT_TRUE(dummy_level->ShouldDraw(object_right));
  }

  {
    const StaticObject object_top = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, 5}, {13, 5}, {3, -1}, {13, -1}}));

    EXPECT_TRUE(dummy_level->ShouldDraw(object_top));
  }

  {
    const StaticObject object_bottom = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, 90}, {13, 90}, {3, 200}, {13, 200}}));

    EXPECT_TRUE(dummy_level->ShouldDraw(object_bottom));
  }
}

TEST_F(LevelTest, DrawsPartiallyOutsideScreen) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();
  dummy_level->screen_edge_objects_.clear();
  constexpr double screen_width = 200;
  constexpr double screen_height = 100;
  const std::unique_ptr<objects::ScreenEdgeObject> screen_left =
      objects::ScreenEdgeObject::MakeLeft(screen_height,
                                          /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_right =
      objects::ScreenEdgeObject::MakeRight(screen_width, screen_height,
                                           /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_top =
      objects::ScreenEdgeObject::MakeTop(screen_width,
                                         /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
      objects::ScreenEdgeObject::MakeBottom(screen_width, screen_height,
                                            /*should_draw_hit_box=*/false);
  dummy_level->screen_edge_objects_.push_back(screen_left.get());
  dummy_level->screen_edge_objects_.push_back(screen_right.get());
  dummy_level->screen_edge_objects_.push_back(screen_top.get());
  dummy_level->screen_edge_objects_.push_back(screen_bottom.get());

  {
    const StaticObject object_left = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{-2, 50}, {-2, 49}, {-1, 50}, {-1, 49}}),
        MakeSprite());
    EXPECT_TRUE(dummy_level->ShouldDraw(object_left));
  }

  {
    const StaticObject object_right = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{201, 50}, {201, 49}, {202, 50}, {202, 49}}),
        MakeSprite());

    EXPECT_TRUE(dummy_level->ShouldDraw(object_right));
  }

  {
    const StaticObject object_top = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, -2}, {13, -2}, {3, -1}, {13, -1}}),
        MakeSprite());

    EXPECT_TRUE(dummy_level->ShouldDraw(object_top));
  }

  {
    const StaticObject object_bottom = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, 101}, {13, 101}, {3, 102}, {13, 102}}),
        MakeSprite());

    EXPECT_TRUE(dummy_level->ShouldDraw(object_bottom));
  }
}

TEST_F(LevelTest, DoesNotDrawOutsideScreenOnlyHitBox) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();
  dummy_level->screen_edge_objects_.clear();
  constexpr double screen_width = 200;
  constexpr double screen_height = 100;
  const std::unique_ptr<objects::ScreenEdgeObject> screen_left =
      objects::ScreenEdgeObject::MakeLeft(screen_height,
                                          /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_right =
      objects::ScreenEdgeObject::MakeRight(screen_width, screen_height,
                                           /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_top =
      objects::ScreenEdgeObject::MakeTop(screen_width,
                                         /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
      objects::ScreenEdgeObject::MakeBottom(screen_width, screen_height,
                                            /*should_draw_hit_box=*/false);
  dummy_level->screen_edge_objects_.push_back(screen_left.get());
  dummy_level->screen_edge_objects_.push_back(screen_right.get());
  dummy_level->screen_edge_objects_.push_back(screen_top.get());
  dummy_level->screen_edge_objects_.push_back(screen_bottom.get());

  {
    const StaticObject object_left = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{-100, 50}, {-100, 49}, {-1, 50}, {-1, 49}}));
    EXPECT_FALSE(dummy_level->ShouldDraw(object_left));
  }

  {
    const StaticObject object_right = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{210, 50}, {210, 49}, {400, 50}, {400, 49}}));

    EXPECT_FALSE(dummy_level->ShouldDraw(object_right));
  }

  {
    const StaticObject object_top = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, -1}, {13, -1}, {3, -100}, {13, -100}}));

    EXPECT_FALSE(dummy_level->ShouldDraw(object_top));
  }

  {
    const StaticObject object_bottom = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, 120}, {13, 120}, {3, 200}, {13, 200}}));

    EXPECT_FALSE(dummy_level->ShouldDraw(object_bottom));
  }
}

TEST_F(LevelTest, DoesNotDrawOutsideScreen) {
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  const std::unique_ptr<DummyLevel> dummy_level =
      dummy_builder.WithScreenObjects().Build();
  dummy_level->screen_edge_objects_.clear();
  constexpr double screen_width = 200;
  constexpr double screen_height = 100;
  const std::unique_ptr<objects::ScreenEdgeObject> screen_left =
      objects::ScreenEdgeObject::MakeLeft(screen_height,
                                          /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_right =
      objects::ScreenEdgeObject::MakeRight(screen_width, screen_height,
                                           /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_top =
      objects::ScreenEdgeObject::MakeTop(screen_width,
                                         /*should_draw_hit_box=*/false);
  const std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
      objects::ScreenEdgeObject::MakeBottom(screen_width, screen_height,
                                            /*should_draw_hit_box=*/false);
  dummy_level->screen_edge_objects_.push_back(screen_left.get());
  dummy_level->screen_edge_objects_.push_back(screen_right.get());
  dummy_level->screen_edge_objects_.push_back(screen_top.get());
  dummy_level->screen_edge_objects_.push_back(screen_bottom.get());

  {
    const StaticObject object_left = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{-100, 50}, {-100, 49}, {-50, 50}, {-50, 49}}),
        MakeSprite());
    EXPECT_FALSE(dummy_level->ShouldDraw(object_left));
  }

  {
    const StaticObject object_right = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{210, 50}, {210, 49}, {400, 50}, {400, 49}}),
        MakeSprite());

    EXPECT_FALSE(dummy_level->ShouldDraw(object_right));
  }

  {
    const StaticObject object_top = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, -1}, {13, -1}, {3, -100}, {13, -100}}),
        MakeSprite());

    EXPECT_FALSE(dummy_level->ShouldDraw(object_top));
  }

  {
    const StaticObject object_bottom = StaticObject(
        /*type=*/ObjectTypeFactory::MakePlayer(),
        StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = false},
        /*hit_box_vertices=*/
        std::vector<std::pair<double, double>>(
            {{3, 120}, {13, 120}, {3, 200}, {13, 200}}),
        MakeSprite());

    EXPECT_FALSE(dummy_level->ShouldDraw(object_bottom));
  }
}

TEST_F(LevelTest, DrawBackground) {
  const std::string resource_path_0 =
      std::filesystem::path("a/b/picture_0.png").make_preferred().string();
  const std::string resource_path_1 =
      std::filesystem::path("a/b/picture_1.png").make_preferred().string();
  constexpr float parallax_factor_0 = 0.5f;
  constexpr float parallax_factor_1 = 0.8f;
  std::unique_ptr<SpriteInstance> sprite_0 =
      sprite_factory_.MakeBackgroundStaticSprite(resource_path_0,
                                                 parallax_factor_0);
  std::unique_ptr<SpriteInstance> sprite_1 =
      sprite_factory_.MakeBackgroundStaticSprite(resource_path_1,
                                                 parallax_factor_1);
  const GraphicsMock* graphics_0 =
      dynamic_cast<const GraphicsMock*>(sprite_0->GraphicsForTesting());
  ASSERT_NE(graphics_0, nullptr);
  const GraphicsMock* graphics_1 =
      dynamic_cast<const GraphicsMock*>(sprite_1->GraphicsForTesting());
  ASSERT_NE(graphics_1, nullptr);
  LevelBuilder<DummyLevel> dummy_builder(kInvalidLevel);
  dummy_builder.AddBackgroundLayer(std::move(sprite_0));
  dummy_builder.AddBackgroundLayer(std::move(sprite_1));
  const std::unique_ptr<DummyLevel> dummy_level = dummy_builder.Build();

  dummy_level->DrawBackgrounds();

  {
    EXPECT_EQ(graphics_0->loaded_texture(), resource_path_0);
    EXPECT_EQ(graphics_0->drawn_texture_source().x, 0);
    EXPECT_EQ(graphics_0->drawn_texture_source().y, 0);
    EXPECT_EQ(graphics_0->drawn_texture_source().width,
              static_cast<float>(graphics_0->ScreenWidth()));
    EXPECT_EQ(graphics_0->drawn_texture_source().height,
              static_cast<float>(graphics_0->ScreenHeight()));
    EXPECT_EQ(graphics_0->drawn_texture_origin().x,
              static_cast<float>(graphics_0->ScreenWidth()) / 2);
    EXPECT_EQ(graphics_0->drawn_texture_origin().y,
              static_cast<float>(graphics_0->ScreenHeight()) / 2);
    EXPECT_EQ(graphics_0->drawn_texture().id, kTextureId);
    EXPECT_EQ(graphics_0->rotation(), 0);
  }
  {
    EXPECT_EQ(graphics_1->loaded_texture(), resource_path_1);
    EXPECT_EQ(graphics_1->drawn_texture_source().x, 0);
    EXPECT_EQ(graphics_1->drawn_texture_source().y, 0);
    EXPECT_EQ(graphics_1->drawn_texture_source().width,
              static_cast<float>(graphics_1->ScreenWidth()));
    EXPECT_EQ(graphics_1->drawn_texture_source().height,
              static_cast<float>(graphics_1->ScreenHeight()));
    EXPECT_EQ(graphics_1->drawn_texture_origin().x,
              static_cast<float>(graphics_1->ScreenWidth()) / 2);
    EXPECT_EQ(graphics_1->drawn_texture_origin().y,
              static_cast<float>(graphics_1->ScreenHeight()) / 2);
    EXPECT_EQ(graphics_1->drawn_texture().id, kTextureId);
    EXPECT_EQ(graphics_1->rotation(), 0);
  }
}

}  // namespace api
}  // namespace lib