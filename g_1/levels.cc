#include "g_1/levels.h"

#include <list>
#include <memory>

#include "g_1/opening_level.h"
#include "g_1/player.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/move_with_cursor_ability.h"
#include "lib/api/abilities/projectile_ability.h"
#include "lib/api/common_types.h"
#include "lib/api/controls.h"
#include "lib/api/factories.h"
#include "lib/api/level.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/projectile_object.h"
#include "lib/api/objects/rectangle_button_object.h"
#include "lib/api/objects/static_object.h"
#include "lib/api/sprites/sprite_factory.h"
#include "lib/api/sprites/sprite_instance.h"
#include "lib/api/text/font_factory.h"
#include "lib/api/text/text.h"
#include "lib/api/title_screen_level.h"

constexpr int kButtonOffsetX = 300;
constexpr int kButtonOffsetY = 500;
constexpr int kButtonLengthX = 400;
constexpr int kButtonLengthY = 100;
constexpr int kOffsetBetweenButtons = 50;

constexpr float kProjectileRadius = 8;
constexpr float kProjectileSpeed = 5;

constexpr float kPlayerSpeed = 7;
constexpr int kPlayerX = 0;
constexpr int kPlayerY = 0;
constexpr float kPlayerWidth = 50;
constexpr float kPlayerHeight = 100;

constexpr float kWorldBorderSize = 10000;

static const char* kTreePath = "g_1/resources/sample_tree.png";
static const char* kPlayerPath = "g_1/resources/sample_player.png";
static const char* kTitleStarsBackground =
    "g_1/resources/title_stars_background.png";
static const char* kOpeningBackground = "g_1/resources/opening_background.png";
static const char* kTitlePlanet1 = "g_1/resources/title_planet_1.png";
static const char* kAsteroid1 = "g_1/resources/asteroid_1.png";
static const char* kAsteroid2 = "g_1/resources/asteroid_2.png";

namespace g_1 {
namespace {

using lib::api::ColorRGBA;
using lib::api::Controls;
using lib::api::Factories;
using lib::api::FCircle;
using lib::api::FRectangle;
using lib::api::kKeyA;
using lib::api::kKeyD;
using lib::api::kKeyS;
using lib::api::kKeyW;
using lib::api::kTitleScreenLevel;
using lib::api::Level;
using lib::api::TitleScreenLevelBuilder;
using lib::api::WorldPosition;
using lib::api::abilities::Ability;
using lib::api::abilities::MoveAbility;
using lib::api::abilities::MoveWithCursorAbility;
using lib::api::abilities::ProjectileAbility;
using lib::api::objects::MovableObject;
using lib::api::objects::Object;
using lib::api::objects::ObjectType;
using lib::api::objects::ObjectTypeFactory;
using lib::api::objects::ProjectileObject;
using lib::api::objects::RectangleButtonObject;
using lib::api::objects::StaticObject;
using lib::api::sprites::SpriteFactory;
using lib::api::sprites::SpriteInstance;
using lib::api::text::FontStyle;
using lib::api::text::Text;
using lib::internal::HitBox;

std::unique_ptr<Player> MakePlayer(SpriteFactory& sprite_factory,
                                   const bool debug_mode) {
  std::unique_ptr<SpriteInstance> player_instance =
      sprite_factory.MakeStaticSprite(kPlayerPath);

  return std::make_unique<Player>(
      /*type=*/ObjectTypeFactory::MakePlayer(),
      /*options=*/
      MovableObject::MovableObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = debug_mode,
                                       .attach_camera = true,
                                       .velocity = kPlayerSpeed},
      FRectangle{.top_left = {kPlayerX, kPlayerY},
                 .width = kPlayerWidth,
                 .height = kPlayerHeight},
      std::move(player_instance));
}

std::list<std::unique_ptr<Ability>> MakePlayerAbilities(const bool debug_mode) {
  std::list<std::unique_ptr<Ability>> abilities;
  abilities.push_back(std::make_unique<MoveAbility>(
      std::make_unique<Controls>(),
      MoveAbility::MoveAbilityOpts{.key_left = kKeyA,
                                   .key_right = kKeyD,
                                   .key_top = kKeyW,
                                   .key_bottom = kKeyS}));

  abilities.push_back(
      std::make_unique<MoveWithCursorAbility>(std::make_unique<Controls>()));

  absl::flat_hash_set<ObjectType> despawn;
  absl::flat_hash_set<ObjectType> reflect;
  absl::flat_hash_set<ObjectType> ignore;
  despawn.insert(ObjectTypeFactory::MakeEnemy());
  reflect.insert(ObjectTypeFactory::MakeButton());
  abilities.push_back(std::make_unique<ProjectileAbility>(
      std::make_unique<Controls>(),
      /*projectile_type=*/ObjectTypeFactory::MakeProjectilePlayer(),
      /*opts=*/ProjectileAbility::ProjectileAbilityOpts{.cooldown_sec = 1},
      /*projectile_object_opts=*/
      ProjectileObject::ProjectileObjectOpts{
          .should_draw_hit_box = debug_mode,
          .despawn_outside_screen_area = true,
          .velocity = kProjectileSpeed,
          .hit_box_center = {-100, -100},
          .hit_box_radius = kProjectileRadius,
          .despawn_on_colliding_with_these_objects = std::move(despawn),
          .reflect_on_colliding_with_these_objects = std::move(reflect),
          .ignore_these_objects = std::move(ignore)}));

  return abilities;
}

std::vector<std::unique_ptr<StaticObject>> MakeStaticObjects(
    SpriteFactory& sprite_factory, const bool debug_mode) {
  std::vector<std::unique_ptr<StaticObject>> static_objects;
  std::unique_ptr<SpriteInstance> tree_1 =
      sprite_factory.MakeStaticSprite(kTreePath);
  std::unique_ptr<SpriteInstance> tree_2 =
      sprite_factory.MakeStaticSprite(kTreePath);
  std::unique_ptr<SpriteInstance> tree_3 =
      sprite_factory.MakeStaticSprite(kTreePath);

  static_objects.push_back(std::make_unique<StaticObject>(
      /*type=*/ObjectTypeFactory::MakeButton(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = debug_mode},
      FRectangle{.top_left = {700, -300}, .width = 100, .height = 200},
      std::move(tree_1)));

  static_objects.push_back(std::make_unique<StaticObject>(
      /*type=*/ObjectTypeFactory::MakeButton(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = debug_mode},
      FRectangle{.top_left = {500, -300}, .width = 100, .height = 200},
      std::move(tree_2)));

  static_objects.push_back(std::make_unique<StaticObject>(
      /*type=*/ObjectTypeFactory::MakeButton(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = debug_mode},
      FRectangle{.top_left = {200, 0}, .width = 100, .height = 200},
      std::move(tree_3)));
  return static_objects;
}

}  // namespace

std::unique_ptr<Level> MakeOpeningLevel(Factories& factories,
                                        const float native_screen_width,
                                        const float native_screen_height,
                                        const bool debug_mode) {
  std::unique_ptr<Player> player = MakePlayer(factories.sprite, debug_mode);
  std::vector<std::unique_ptr<StaticObject>> static_objects =
      MakeStaticObjects(factories.sprite, debug_mode);

  OpeningLevelBuilder level_builder = OpeningLevelBuilder(
      lib::api::kFirstLevel, native_screen_width, native_screen_height);
  level_builder.AddPlayerAndAbilities(std::move(player),
                                      MakePlayerAbilities(debug_mode));
  for (auto& static_object : static_objects) {
    level_builder.AddObject(std::move(static_object));
  }
  level_builder.WithScreenObjects(/*should_draw_hitbox=*/false);
  if (debug_mode) {
    level_builder.WithCoordinates();
  }
  level_builder.WithWorldBorderX(-kWorldBorderSize,
                                 /*should_draw_hitbox=*/debug_mode);
  level_builder.WithWorldBorderX(kWorldBorderSize,
                                 /*should_draw_hitbox=*/debug_mode);
  level_builder.WithWorldBorderY(-kWorldBorderSize,
                                 /*should_draw_hitbox=*/debug_mode);
  level_builder.WithWorldBorderY(kWorldBorderSize,
                                 /*should_draw_hitbox=*/debug_mode);
  level_builder.AddBackgroundLayer(factories.sprite.MakeBackgroundStaticSprite(
      kOpeningBackground, /*parallax_factor=*/0.2f));
  return level_builder.Build();
}

std::unique_ptr<Level> MakeTitleScreenLevel(Factories& factories,
                                            const float native_screen_width,
                                            const float native_screen_height,
                                            const bool debug_mode) {
  std::unique_ptr<RectangleButtonObject> start_button =
      std::make_unique<RectangleButtonObject>(
          /*type=*/
          ObjectTypeFactory::MakeButton(),
          FRectangle{.top_left = {kButtonOffsetX, kButtonOffsetY},
                     .width = kButtonLengthX,
                     .height = kButtonLengthY},
          Text(factories.font.MakeRoboto(FontStyle::BOLD), "Start",
               /*color=*/ColorRGBA::MakeWhite(),
               /*spacing=*/0.0f,
               /*font_size=*/48.0f),
          RectangleButtonObject::RectangleButtonObjectOpts{
              .has_round_corners = false,
              .border_thickness = 7.0f,
              .border_color = ColorRGBA::MakeWhite(),
              .fill_color = ColorRGBA::MakeTransparent()});
  std::unique_ptr<RectangleButtonObject> exit_button =
      std::make_unique<RectangleButtonObject>(
          /*type=*/
          ObjectTypeFactory::MakeButton(),
          FRectangle{
              .top_left = {kButtonOffsetX, kButtonOffsetY + kButtonLengthY +
                                               kOffsetBetweenButtons},
              .width = kButtonLengthX,
              .height = kButtonLengthY},
          Text(factories.font.MakeRoboto(FontStyle::BOLD), "Exit",
               /*color=*/ColorRGBA::MakeWhite(),
               /*spacing=*/0.0f,
               /*font_size=*/48.0f),
          RectangleButtonObject::RectangleButtonObjectOpts{
              .has_round_corners = false,
              .border_thickness = 7.0f,
              .border_color = ColorRGBA::MakeWhite(),
              .fill_color = ColorRGBA::MakeTransparent()});

  std::unique_ptr<SpriteInstance> title_planet_1_sprite =
      factories.sprite.MakeStaticSprite(kTitlePlanet1);
  std::unique_ptr<SpriteInstance> asteroid_1_sprite =
      factories.sprite.MakeStaticSprite(kAsteroid1);
  std::unique_ptr<SpriteInstance> asteroid_2_sprite =
      factories.sprite.MakeStaticSprite(kAsteroid2);

  TitleScreenLevelBuilder level_builder =
      TitleScreenLevelBuilder(native_screen_width, native_screen_height);
  return level_builder.AddStartButton(std::move(start_button))
      .AddExitButton(std::move(exit_button))
      .AddBackgroundLayer(factories.sprite.MakeBackgroundStaticSprite(
          kTitleStarsBackground, /*parallax_factor=*/0.0))
      .AddObject(std::make_unique<StaticObject>(
          /*type=*/ObjectTypeFactory::MakeEnemy(),
          StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                         .should_draw_hit_box = debug_mode},
          FCircle{.center = {1700, 800}, .radius = 10},
          std::move(title_planet_1_sprite)))
      .AddObject(std::make_unique<StaticObject>(
          /*type=*/ObjectTypeFactory::MakeEnemy(),
          StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                         .should_draw_hit_box = debug_mode},
          FCircle{.center = {1200, 200}, .radius = 10},
          std::move(asteroid_1_sprite)))
      .AddObject(std::make_unique<StaticObject>(
          /*type=*/ObjectTypeFactory::MakeEnemy(),
          StaticObject::StaticObjectOpts{.is_hit_box_active = false,
                                         .should_draw_hit_box = debug_mode},
          FCircle{.center = {1000, 300}, .radius = 10},
          std::move(asteroid_2_sprite)))
      .Build();
}

}  // namespace g_1
