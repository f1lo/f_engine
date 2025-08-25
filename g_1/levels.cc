#include "g_1/levels.h"

#include <list>

#include "g_1/opening_level.h"
#include "g_1/player.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/abilities/move_with_cursor_ability.h"
#include "lib/api/abilities/projectile_ability.h"
#include "lib/api/level.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/projectile_object.h"
#include "lib/api/objects/static_object.h"
#include "lib/api/title_screen_level.h"

constexpr int kButtonOffsetX = 300;
constexpr int kButtonOffsetY = 500;
constexpr int kButtonLengthX = 400;
constexpr int kButtonLengthY = 100;
constexpr int kOffsetBetweenButtons = 50;

constexpr double kProjectileRadius = 20;
constexpr double kProjectileSpeed = 5;

constexpr double kPlayerSpeed = 7;
constexpr int kPlayerX = 0;
constexpr int kPlayerY = 0;
constexpr float kPlayerWidth = 20;
constexpr float kPlayerHeight = 20;

namespace g_1 {
namespace {

using lib::api::kTitleScreenLevel;
using lib::api::Level;
using lib::api::TitleScreenLevelBuilder;
using lib::api::abilities::Ability;
using lib::api::abilities::kKeyA;
using lib::api::abilities::kKeyD;
using lib::api::abilities::kKeyS;
using lib::api::abilities::kKeyW;
using lib::api::abilities::MoveAbility;
using lib::api::abilities::MoveWithCursorAbility;
using lib::api::abilities::ProjectileAbility;
using lib::api::objects::kButton;
using lib::api::objects::kEnemy;
using lib::api::objects::Kind;
using lib::api::objects::kPlayer;
using lib::api::objects::kProjectilePlayer;
using lib::api::objects::MovableObject;
using lib::api::objects::Object;
using lib::api::objects::ProjectileObject;
using lib::api::objects::StaticObject;
using lib::internal::HitBox;

std::unique_ptr<Player> MakePlayer(const bool debug_mode) {
  return std::make_unique<Player>(
      /*kind=*/kPlayer,
      /*options=*/
      MovableObject::MovableObjectOpts(
          /*is_hit_box_active*/ true, /*should_draw_hitbox*/ debug_mode,
          /*attach_camera=*/true, kPlayerSpeed), /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{kPlayerX, kPlayerY},
           {kPlayerX, kPlayerY + kPlayerHeight},
           {kPlayerX + kPlayerWidth, kPlayerY},
           {kPlayerX + kPlayerWidth, kPlayerY + kPlayerHeight}}));
}

std::list<std::unique_ptr<Ability>> MakePlayerAbilities(const bool debug_mode) {
  std::list<std::unique_ptr<Ability>> abilities;
  abilities.push_back(std::make_unique<MoveAbility>(
      MoveAbility::MoveAbilityOpts(Ability::AbilityOpts(/*cooldown_sec=*/0),
                                   /*key_left=*/kKeyA, /*key_right=*/kKeyD,
                                   /*key_top=*/kKeyW, /*key_bottom=*/kKeyS)));

  abilities.push_back(std::make_unique<MoveWithCursorAbility>());

  absl::flat_hash_set<Kind> despawn;
  absl::flat_hash_set<Kind> reflect;
  absl::flat_hash_set<Kind> ignore;
  despawn.insert(kEnemy);
  reflect.insert(kButton);
  abilities.push_back(std::make_unique<ProjectileAbility>(
      /*projectile_kind=*/kProjectilePlayer,
      /*opts=*/ProjectileAbility::ProjectileAbilityOpts(/*cooldown_sec=*/1),
      /*projectile_object_opts=*/
      ProjectileObject::ProjectileObjectOpts(
          /*should_draw_hit_box=*/debug_mode,
          /*despawn_outside_screen_area=*/true,
          /*velocity=*/kProjectileSpeed, /*hit_box_center=*/{-100, -100},
          /*hit_box_radius=*/kProjectileRadius,
          /*despawn_on_colliding_with_these_objects=*/std::move(despawn),
          /*reflect_on_colliding_with_these_objects=*/std::move(reflect),
          /*ignore_these_objects=*/std::move(ignore))));

  return abilities;
}

std::vector<std::unique_ptr<StaticObject>> MakeStaticObjects(
    const bool debug_mode) {
  std::vector<std::unique_ptr<StaticObject>> static_objects;
  static_objects.push_back(std::make_unique<StaticObject>(
      /*kind=*/kButton,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/true,
          /*should_draw_hit_box=*/debug_mode),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{kButtonOffsetX, kButtonOffsetY},
           {kButtonOffsetX, kButtonOffsetY + kButtonLengthY},
           {kButtonOffsetX + kButtonLengthX, kButtonOffsetY},
           {kButtonOffsetX + kButtonLengthX,
            kButtonOffsetY + kButtonLengthY}})));
  static_objects.push_back(std::make_unique<StaticObject>(
      /*kind=*/kEnemy,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/true,
          /*should_draw_hit_box=*/debug_mode),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{500, -200}, {500, 200}, {1000, -200}, {1000, 200}})));

  return static_objects;
}

}  // namespace

std::unique_ptr<Level> MakeOpeningLevel(const bool debug_mode) {
  std::unique_ptr<Player> player = MakePlayer(debug_mode);
  std::vector<std::unique_ptr<StaticObject>> static_objects =
      MakeStaticObjects(debug_mode);

  OpeningLevelBuilder level_builder =
      OpeningLevelBuilder(lib::api::kFirstLevel);
  level_builder.AddPlayerAndAbilities(std::move(player),
                                      MakePlayerAbilities(debug_mode));
  for (auto& static_object : static_objects) {
    level_builder.AddObject(std::move(static_object));
  }
  level_builder.WithScreenObjects(/*should_draw_hitbox=*/debug_mode);
  return level_builder.Build();
}

std::unique_ptr<Level> MakeTitleScreenLevel(bool debug_mode) {
  std::unique_ptr<StaticObject> start_button = std::make_unique<StaticObject>(
      /*kind=*/kButton,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/true,
          /*should_draw_hit_box=*/debug_mode),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{kButtonOffsetX, kButtonOffsetY},
           {kButtonOffsetX, kButtonOffsetY + kButtonLengthY},
           {kButtonOffsetX + kButtonLengthX, kButtonOffsetY},
           {kButtonOffsetX + kButtonLengthX,
            kButtonOffsetY + kButtonLengthY}}));
  std::unique_ptr<StaticObject> exit_button = std::make_unique<StaticObject>(
      /*kind=*/kButton,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/true,
          /*should_draw_hit_box=*/debug_mode),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{kButtonOffsetX,
            kButtonOffsetY + kButtonLengthY + kOffsetBetweenButtons},
           {kButtonOffsetX,
            kButtonOffsetY + 2 * kButtonLengthY + kOffsetBetweenButtons},
           {kButtonOffsetX + kButtonLengthX,
            kButtonOffsetY + kButtonLengthY + kOffsetBetweenButtons},
           {kButtonOffsetX + kButtonLengthX,
            kButtonOffsetY + 2 * kButtonLengthY + kOffsetBetweenButtons}}));

  TitleScreenLevelBuilder level_builder;
  return level_builder.AddStartButton(std::move(start_button))
      .AddExitButton(std::move(exit_button))
      .Build();
}

}  // namespace g_1
