#include <list>
#include <memory>
#include <optional>

#include "examples/breakout/ball.h"
#include "examples/breakout/ball_ability.h"
#include "examples/breakout/brick.h"
#include "examples/breakout/level_main.h"
#include "examples/breakout/player_pad.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/game.h"
#include "lib/api/level.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"

using breakout::BrickObject;
using breakout::LevelMain;
using breakout::LevelMainBuilder;
using breakout::PlayerPad;
using lib::api::Game;
using lib::api::Level;
using lib::api::abilities::Ability;
using lib::api::abilities::kKeyA;
using lib::api::abilities::kKeyD;
using lib::api::abilities::kKeyS;
using lib::api::abilities::kKeyW;
using lib::api::abilities::MoveAbility;
using lib::api::objects::kEnemy;
using lib::api::objects::MovableObject;
using lib::api::objects::Object;
using lib::api::objects::StaticObject;

constexpr int kPlayerWidth = 300;
constexpr int kPlayerHeight = 25;
constexpr int kScreenOffset = 10;
constexpr int kBrickWidth = 130;
constexpr int kBrickHeight = 30;
constexpr int kBallVelocity = 9;
constexpr int kBallRadius = 20;

std::vector<std::unique_ptr<BrickObject>> GenerateBricks(int brick_width,
                                                         int brick_height,
                                                         int screen_width,
                                                         int screen_height,
                                                         int num_brick_lines) {
  std::vector<std::unique_ptr<BrickObject>> bricks;
  int usable_space = screen_width - 2 * kScreenOffset;
  int brick_offset_close =
      (usable_space - brick_width * (usable_space / brick_width)) / 2 +
      kScreenOffset;
  int brick_offset_far = brick_offset_close + brick_width / 2;
  int right_limit = kScreenOffset + usable_space;

  if (screen_height / 2 < num_brick_lines * brick_height) {
    std::cout << "Too many brick lines!\n";
    return {};
  }

  for (int i = 0; i < num_brick_lines; i++) {
    double x = (i % 2 == 0) ? brick_offset_close : brick_offset_far;
    double y = kScreenOffset + (i + 1) * brick_height;
    while (x + brick_width <= right_limit) {
      std::unique_ptr<BrickObject> brick = std::make_unique<BrickObject>(
          kEnemy,
          StaticObject::StaticObjectOpts(/*is_hit_box_active*/ true,
                                         /*should_draw_hitbox*/ true),
          std::vector<std::pair<double, double>>(
              {{x, y},
               {x, y - brick_height},
               {x + brick_width, y - brick_height},
               {x + brick_width, y}}));
      bricks.emplace_back(std::move(brick));
      x += brick_width;
    }
  }

  return bricks;
}

int main() {
  Game& game = Game::Create(/*width=*/1500, /*height=*/1000, "Breakout",
                            /*full_screen=*/false);

  MoveAbility::MoveAbilityOpts opts = MoveAbility::MoveAbilityOpts(
      {.cooldown_sec = 0}, kKeyA, kKeyD, std::nullopt, std::nullopt);
  std::unique_ptr<MoveAbility> ability_move =
      std::make_unique<MoveAbility>(opts);
  std::unique_ptr<breakout::BallAbility> ability_ball =
      std::make_unique<breakout::BallAbility>(lib::api::abilities::kKeySpace);

  std::unique_ptr<Object> player = std::make_unique<PlayerPad>(
      game.screen_width(), game.screen_height(), kPlayerWidth, kPlayerHeight,
      MovableObject::MovableObjectOpts(
          /*is_hit_box_active=*/true, /*should_draw_hit_box=*/true,
          /*attach_camera=*/false, /*velocity=*/8));

  std::list<std::unique_ptr<Ability>> player_abilities;
  player_abilities.push_back(std::move(ability_move));
  std::unique_ptr<breakout::Ball> ball = std::make_unique<breakout::Ball>(
      MovableObject::MovableObjectOpts(
          /*is_hit_box_active=*/true, /*should_draw_hit_box=*/true,
          /*attach_camera=*/false, /*velocity=*/kBallVelocity),
      /*hit_box_center=*/
      std::make_pair(game.screen_width() / 2,
                     game.screen_height() - breakout::kPadOffset -
                         kPlayerHeight - breakout::kBallOffset - kBallRadius),
      kBallRadius);
  std::list<std::unique_ptr<Ability>> ball_abilities;
  ball_abilities.push_back(std::move(ability_ball));
  LevelMainBuilder level_builder =
      LevelMainBuilder(/*id=*/lib::api::kTitleScreenLevel, ball.get());
  level_builder
      .AddObjectAndAbilities(std::move(player), std::move(player_abilities))
      .AddObjectAndAbilities(std::move(ball), std::move(ball_abilities))
      .WithScreenObjects();

  std::vector<std::unique_ptr<BrickObject>> bricks =
      GenerateBricks(kBrickWidth, kBrickHeight, game.screen_width(),
                     game.screen_height(), /*num_brick_lines=*/5);
  for (auto& brick : bricks) {
    level_builder.AddObject(std::move(brick));
  }
  game.AddLevel(std::move(level_builder.Build()));
  game.Run();
}
