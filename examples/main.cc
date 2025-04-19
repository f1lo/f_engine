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

std::optional<Object::PendingUpdate> LeftScreenCollisionCallback(
    const Object& to_update, const Object& other) {
  Object::PendingUpdate update = {
      .deleted = false,
      .is_hit_box_active = true,
      .velocity_x = std::nullopt,
      .velocity_y = std::nullopt,
      .velocity_x_add = std::nullopt,
      .velocity_y_add = std::nullopt,
      .velocity_x_multiply = -1,
      .velocity_y_multiply = 1,
  };
  return update;
}

std::optional<Object::PendingUpdate> RightScreenCollisionCallback(
    const Object& to_update, const Object& other) {
  Object::PendingUpdate update = {
      .deleted = false,
      .is_hit_box_active = true,
      .velocity_x = std::nullopt,
      .velocity_y = std::nullopt,
      .velocity_x_add = std::nullopt,
      .velocity_y_add = std::nullopt,
      .velocity_x_multiply = -1,
      .velocity_y_multiply = 1,
  };
  return update;
}

std::optional<Object::PendingUpdate> TopScreenCollisionCallback(
    const Object& to_update, const Object& other) {
  Object::PendingUpdate update = {
      .deleted = false,
      .is_hit_box_active = true,
      .velocity_x = std::nullopt,
      .velocity_y = std::nullopt,
      .velocity_x_add = std::nullopt,
      .velocity_y_add = std::nullopt,
      .velocity_x_multiply = 1,
      .velocity_y_multiply = -1,
  };
  return update;
}

std::optional<Object::PendingUpdate> BottomScreenCollisionCallback(
    const Object& to_update, const Object& other) {
  Object::PendingUpdate update = {
      .deleted = false,
      .is_hit_box_active = true,
      .velocity_x = std::nullopt,
      .velocity_y = std::nullopt,
      .velocity_x_add = std::nullopt,
      .velocity_y_add = std::nullopt,
      .velocity_x_multiply = 1,
      .velocity_y_multiply = -1,
  };
  return update;
}

int main() {
  Game& game = Game::Create();

  std::unique_ptr<MoveAbility> ability_a = std::make_unique<MoveAbility>(
      Ability::AbilityOpts(/*cooldown_sec*/ 0), kKeyA, -5, 0);
  std::unique_ptr<MoveAbility> ability_d = std::make_unique<MoveAbility>(
      Ability::AbilityOpts(/*cooldown_sec*/ 0), kKeyD, 5, 0);
  std::unique_ptr<MoveAbility> ability_w = std::make_unique<MoveAbility>(
      Ability::AbilityOpts(/*cooldown_sec*/ 0), kKeyS, 0, 5);
  std::unique_ptr<MoveAbility> ability_s = std::make_unique<MoveAbility>(
      Ability::AbilityOpts(/*cooldown_sec*/ 0), kKeyW, 0, -5);

  absl::flat_hash_map<Object::Kind, Object::CollisionCallback> callbacks;
  callbacks[Object::Kind::SCREEN_LEFT] = LeftScreenCollisionCallback;
  callbacks[Object::Kind::SCREEN_RIGHT] = RightScreenCollisionCallback;
  callbacks[Object::Kind::SCREEN_TOP] = TopScreenCollisionCallback;
  callbacks[Object::Kind::SCREEN_BOTTOM] = BottomScreenCollisionCallback;
  std::unique_ptr<Object> player = std::make_unique<PlayerControllableObject>(
      Object::Kind::PLAYER,
      PlayerControllableObject::PlayerControllableObjectOpts(true, true, 0, 0),
      std::move(callbacks), std::make_pair(250, 250), 20);

  std::list<std::unique_ptr<Ability>> abilities;
  abilities.push_back(std::move(ability_a));
  abilities.push_back(std::move(ability_d));
  abilities.push_back(std::move(ability_s));
  abilities.push_back(std::move(ability_w));
  std::unique_ptr<Level> level = std::make_unique<Level>();
  level->add_object_and_abilities(std::move(player), std::move(abilities));
  level->AddScreenObjects();
  std::list<std::unique_ptr<Level>> levels;
  levels.push_back(std::move(level));

  game.set_levels(std::move(levels));
  game.Run();
}
