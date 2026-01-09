#include <list>
#include <memory>
#include <optional>

#include "examples/breakout/ball.h"
#include "examples/breakout/ball_ability.h"
#include "examples/breakout/brick.h"
#include "examples/breakout/level_main.h"
#include "examples/breakout/player_pad.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/common_types.h"
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
using lib::api::FCircle;
using lib::api::FRectangle;
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

std::vector<std::unique_ptr<BrickObject>> GenerateBricks(
    const int brick_width, const int brick_height, const int screen_width,
    const int screen_height, const int num_brick_lines) {
  std::vector<std::unique_ptr<BrickObject>> bricks;
  const int usable_space = screen_width - 2 * kScreenOffset;
  const int brick_offset_close =
      (usable_space - brick_width * (usable_space / brick_width)) / 2 +
      kScreenOffset;
  const int brick_offset_far = brick_offset_close + brick_width / 2;
  const int right_limit = kScreenOffset + usable_space;

  if (screen_height / 2 < num_brick_lines * brick_height) {
    std::cout << "Too many brick lines!\n";
    return {};
  }

  for (int i = 0; i < num_brick_lines; i++) {
    float x = (i % 2 == 0) ? static_cast<float>(brick_offset_close)
                           : static_cast<float>(brick_offset_far);
    const float y = static_cast<float>(kScreenOffset) +
                    static_cast<float>((i + 1) * brick_height);
    while (x + static_cast<float>(brick_width) <=
           static_cast<float>(right_limit)) {
      std::unique_ptr<BrickObject> brick = std::make_unique<BrickObject>(
          ObjectTypeFactory::MakeEnemy(),
          StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                         .should_draw_hit_box = true},
          FRectangle{.top_left = {x, y - static_cast<float>(brick_height)},
                     .width = static_cast<float>(brick_width),
                     .height = static_cast<float>(brick_height)});
      bricks.emplace_back(std::move(brick));
      x += static_cast<float>(brick_width);
    }
  }

  return bricks;
}

int main() {
  Game& game = Game::Create({
      .native_screen_width = 1920,
      .native_screen_height = 1080,
      .screen_width = 2200,
      .screen_height = 1280,
      .full_screen = true,
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

  const ObjectType ball_type = game.factories().object_type.MakeNewObjectType();
  std::unique_ptr<Object> player = std::make_unique<PlayerPad>(
      game.native_screen_width(), game.native_screen_height(), kPlayerWidth,
      kPlayerHeight, ball_type,
      MovableObject::MovableObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = true,
                                       .attach_camera = false,
                                       .velocity = 8.0f});

  std::list<std::unique_ptr<Ability>> player_abilities;
  player_abilities.push_back(std::move(ability_move));
  std::unique_ptr<breakout::Ball> ball = std::make_unique<breakout::Ball>(
      ball_type,
      MovableObject::MovableObjectOpts{.is_hit_box_active = true,
                                       .should_draw_hit_box = true,
                                       .attach_camera = false,
                                       .velocity = kBallVelocity},

      FCircle{.center = {static_cast<float>(game.native_screen_width()) / 2.0f,
                         static_cast<float>(game.native_screen_height()) -
                             breakout::kPadOffset - kPlayerHeight -
                             breakout::kBallOffset - kBallRadius},
              .radius = kBallRadius});
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
