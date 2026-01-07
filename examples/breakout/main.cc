#include <list>
#include <memory>
#include <optional>

#include "examples/breakout/ball.h"
#include "examples/breakout/ball_ability.h"
#include "examples/breakout/brick.h"
#include "examples/breakout/level_main.h"
#include "examples/breakout/player_pad.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/controls.h"
#include "lib/api/game.h"
#include "lib/api/level.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

using breakout::BrickObject;
using breakout::LevelMain;
using breakout::LevelMainBuilder;
using breakout::PlayerPad;
using lib::api::Controls;
using lib::api::Game;
using lib::api::kKeyA;
using lib::api::kKeyD;
using lib::api::kKeyS;
using lib::api::kKeyW;
using lib::api::Level;
using lib::api::abilities::Ability;
using lib::api::abilities::MoveAbility;
using lib::api::objects::MovableObject;
using lib::api::objects::Object;
using lib::api::objects::ObjectType;
using lib::api::objects::ObjectTypeFactory;
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
    float x = (i % 2 == 0) ? brick_offset_close : brick_offset_far;
    float y = kScreenOffset + (i + 1) * brick_height;
    while (x + brick_width <= right_limit) {
      std::unique_ptr<BrickObject> brick = std::make_unique<BrickObject>(
          ObjectTypeFactory::MakeEnemy(),
          StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                         .should_draw_hit_box = true},
          std::vector<std::pair<float, float>>(
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
  Game& game = Game::Create({
      .native_screen_width = 1920,
      .native_screen_height = 1080,
      .screen_width = 2500,
      .screen_height = 1500,
      .full_screen = false,
      .title = "Breakout",
  });

  MoveAbility::MoveAbilityOpts opts = {.key_left = kKeyA,
                                       .key_right = kKeyD,
                                       .key_top = std::nullopt,
                                       .key_bottom = std::nullopt};
  std::unique_ptr<MoveAbility> ability_move =
      std::make_unique<MoveAbility>(std::make_unique<Controls>(), opts);
  std::unique_ptr<breakout::BallAbility> ability_ball =
      std::make_unique<breakout::BallAbility>(lib::api::kKeySpace);

  const ObjectType ball_type = game.object_type_factory().MakeNewObjectType();
  std::unique_ptr<Object> player = std::make_unique<PlayerPad>(
      game.native_screen_width(), game.native_screen_height(), kPlayerWidth,
      kPlayerHeight, ball_type,
      MovableObject::MovableObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = true,
                                       .attach_camera = false,
                                       .velocity = 8});

  std::list<std::unique_ptr<Ability>> player_abilities;
  player_abilities.push_back(std::move(ability_move));
  std::unique_ptr<breakout::Ball> ball = std::make_unique<breakout::Ball>(
      ball_type,
      MovableObject::MovableObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = true,
                                       .attach_camera = false,
                                       .velocity = kBallVelocity},
      /*hit_box_center=*/
      std::make_pair(game.native_screen_width() / 2,
                     game.native_screen_height() - breakout::kPadOffset -
                         kPlayerHeight - breakout::kBallOffset - kBallRadius),
      kBallRadius);
  std::list<std::unique_ptr<Ability>> ball_abilities;
  ball_abilities.push_back(std::move(ability_ball));
  LevelMainBuilder level_builder = LevelMainBuilder(
      /*id=*/lib::api::kTitleScreenLevel,
      static_cast<float>(game.native_screen_width()),
      static_cast<float>(game.native_screen_height()), ball.get());
  level_builder
      .AddObjectAndAbilities(std::move(player), std::move(player_abilities))
      .AddObjectAndAbilities(std::move(ball), std::move(ball_abilities))
      .WithScreenObjects();

  std::vector<std::unique_ptr<BrickObject>> bricks =
      GenerateBricks(kBrickWidth, kBrickHeight, game.native_screen_width(),
                     game.native_screen_height(), /*num_brick_lines=*/5);
  for (auto& brick : bricks) {
    level_builder.AddObject(std::move(brick));
  }
  game.AddLevel(std::move(level_builder.Build()));
  game.Run();
}
