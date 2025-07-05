#include "g_1/levels.h"

#include <list>

#include "g_1/opening_level.h"
#include "g_1/player.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/level.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"
#include "lib/api/title_screen_level.h"

using lib::api::kTitleScreenLevel;
using lib::api::Level;
using lib::api::objects::StaticObject;
using lib::internal::HitBox;

constexpr int kButtonOffsetX = 300;
constexpr int kButtonOffsetY = 500;
constexpr int kButtonLengthX = 400;
constexpr int kButtonLengthY = 100;
constexpr int kOffsetBetweenButtons = 50;

constexpr double kPlayerSpeedX = 7.0;
constexpr double kPlayerSpeedY = 7.0;

constexpr float kPlayerWidth = 20;
constexpr float kPlayerHeight = 20;
constexpr float kPlayerOffset = 20;

namespace g_1 {
namespace {

using lib::api::abilities::Ability;
using lib::api::abilities::kKeyA;
using lib::api::abilities::kKeyD;
using lib::api::abilities::kKeyS;
using lib::api::abilities::kKeyW;
using lib::api::abilities::MoveAbility;
using lib::api::objects::MovableObject;
using lib::api::objects::Object;
using lib::api::objects::StaticObject;

std::unique_ptr<Player> MakePlayer(int screen_width, int screen_height,
                                   bool debug_mode) {
  return std::make_unique<Player>(
      /*kind=*/kPlayer,
      /*options=*/
      MovableObject::MovableObjectOpts(
          /*is_hit_box_active*/ true, /*should_draw_hitbox*/ debug_mode,
          kPlayerSpeedX, kPlayerSpeedY), /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{kPlayerOffset, screen_height - kPlayerOffset},
           {kPlayerOffset, screen_height - kPlayerOffset - kPlayerHeight},
           {kPlayerOffset + kPlayerWidth, screen_height - kPlayerOffset},
           {kPlayerOffset + kPlayerWidth,
            screen_height - kPlayerOffset - kPlayerHeight}}));
}

std::list<std::unique_ptr<Ability>> MakePlayerAbilities() {
  std::list<std::unique_ptr<Ability>> abilities;
  abilities.emplace_back(
      std::make_unique<MoveAbility>(MoveAbility::MoveAbilityOpts(
          Ability::AbilityOpts(/*cooldown_sec=*/0), /*should_hold=*/true,
          kPlayerSpeedX, kPlayerSpeedY,
          /*key_left=*/kKeyA, /*key_right=*/kKeyD,
          /*key_top=*/kKeyW, /*key_bottom=*/kKeyS)));
  return std::move(abilities);
}

}  // namespace

std::unique_ptr<Level> MakeOpeningLevel(int screen_width, int screen_height,
                                        bool debug_mode) {
  std::unique_ptr<Player> player =
      MakePlayer(screen_width, screen_height, debug_mode);
  OpeningLevelBuilder level_builder =
      OpeningLevelBuilder(lib::api::kFirstLevel);
  level_builder.SetPlayer(player.get());
  level_builder.AddObjectAndAbilities(std::move(player), MakePlayerAbilities());
  return level_builder.Build();
}

std::unique_ptr<Level> MakeTitleScreenLevel(int screen_width, int screen_height,
                                            bool debug_mode) {
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

  return std::make_unique<lib::api::TitleScreenLevel>(
      kTitleScreenLevel, std::move(start_button), std::move(exit_button));
}

}  // namespace g_1
