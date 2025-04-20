#include <list>
#include <memory>
#include <optional>

#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/game.h"
#include "lib/api/level.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/player_controllable_object.h"
#include "lib/api/objects/static_object.h"

using lib::api::Game;
using lib::api::Level;
using lib::api::abilities::Ability;
using lib::api::abilities::kKeyA;
using lib::api::abilities::kKeyD;
using lib::api::abilities::kKeyS;
using lib::api::abilities::kKeyW;
using lib::api::abilities::MoveAbility;
using lib::api::objects::Object;
using lib::api::objects::PlayerControllableObject;
using lib::api::objects::StaticObject;

constexpr int kPlayerWidth = 200;
constexpr int kPlayerHeight = 30;
constexpr int kScreenOffset = 10;
constexpr int kBrickWidth = 130;
constexpr int kBrickHeight = 30;

std::vector<std::unique_ptr<StaticObject>> GenerateBricks(int brick_width,
                                                          int brick_height,
                                                          int screen_width,
                                                          int screen_height,
                                                          int num_brick_lines) {
  std::vector<std::unique_ptr<StaticObject>> bricks;
  int usable_space = screen_width - 2 * kScreenOffset;
  int brick_offset_close =
      usable_space - brick_width * (usable_space / brick_width);
  int brick_offset_far = brick_offset_close + brick_width / 2;
  int right_limit = kScreenOffset + usable_space;

  if (screen_height / 2 < num_brick_lines * brick_height) {
    std::cout << "Too many brick lines!\n";
    return {};
  }

  for (int i = 0; i < num_brick_lines; i++) {
    int x = (i % 2 == 0) ? brick_offset_close : brick_offset_far;
    int y = kScreenOffset + (i + 1) * brick_height;
    while (x + brick_width <= right_limit) {
      std::unique_ptr<StaticObject> brick = std::make_unique<StaticObject>(
          Object::Kind::ENEMY,
          StaticObject::StaticObjectOpts(/*is_hit_box_active*/ true,
                                         /*should_draw_hitbox*/ true),
          std::vector<std::pair<int, int>>({{x, y},
                                            {x, y - brick_height},
                                            {x + brick_width, y - brick_height},
                                            {x + brick_width, y}}));
      bricks.emplace_back(std::move(brick));
      x += brick_width;
    }
  }

  return std::move(bricks);
}

int main() {
  Game& game = Game::Create(/*width=*/1500, /*height=*/1000, "Breakout",
                            /*full_screen=*/false);

  MoveAbility::MoveAbilityOpts opts = MoveAbility::MoveAbilityOpts(
      {.cooldown_sec = 0}, /*should_hold=*/true, /*velocity_x=*/8,
      /*velocity_y=*/0, kKeyA, kKeyD, kKeyW, kKeyS);
  std::unique_ptr<MoveAbility> ability_move =
      std::make_unique<MoveAbility>(opts);

  std::unique_ptr<Object> player = std::make_unique<PlayerControllableObject>(
      Object::Kind::PLAYER,
      PlayerControllableObject::PlayerControllableObjectOpts(true, true, 0, 0),
      std::vector<std::pair<int, int>>(
          {{game.screen_width() / 2 - kPlayerWidth / 2,
            game.screen_height() - kScreenOffset},
           {game.screen_width() / 2 - kPlayerWidth / 2,
            game.screen_height() - kPlayerHeight - kScreenOffset},
           {game.screen_width() / 2 + kPlayerWidth / 2,
            game.screen_height() - kPlayerHeight - kScreenOffset},
           {game.screen_width() / 2 + kPlayerWidth / 2,
            game.screen_height() - kScreenOffset}}));

  std::list<std::unique_ptr<Ability>> abilities;
  abilities.push_back(std::move(ability_move));
  std::unique_ptr<Level> level = std::make_unique<Level>();
  level->add_object_and_abilities(std::move(player), std::move(abilities));
  level->AddScreenObjects();

  std::vector<std::unique_ptr<StaticObject>> bricks =
      GenerateBricks(kBrickWidth, kBrickHeight, game.screen_width(),
                     game.screen_height(), /*num_brick_lines=*/5);
  for (auto& brick : bricks) {
    level->add_object(std::move(brick));
  }

  std::list<std::unique_ptr<Level>> levels;
  levels.push_back(std::move(level));

  game.set_levels(std::move(levels));
  game.Run();
}
