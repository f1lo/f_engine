#include <list>
#include <memory>

#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/game.h"
#include "lib/api/level.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/player_controllable_object.h"

int main() {
  std::unique_ptr<lib::api::abilities::MoveAbility> ability_a =
      std::make_unique<lib::api::abilities::MoveAbility>(
          lib::api::abilities::Ability::AbilityOpts(/*cooldown_sec*/ 0),
          lib::api::abilities::kKeyA, -5, 0);
  std::unique_ptr<lib::api::abilities::MoveAbility> ability_d =
      std::make_unique<lib::api::abilities::MoveAbility>(
          lib::api::abilities::Ability::AbilityOpts(/*cooldown_sec*/ 0),
          lib::api::abilities::kKeyD, 5, 0);
  std::unique_ptr<lib::api::abilities::MoveAbility> ability_w =
      std::make_unique<lib::api::abilities::MoveAbility>(
          lib::api::abilities::Ability::AbilityOpts(/*cooldown_sec*/ 0),
          lib::api::abilities::kKeyS, 0, 5);
  std::unique_ptr<lib::api::abilities::MoveAbility> ability_s =
      std::make_unique<lib::api::abilities::MoveAbility>(
          lib::api::abilities::Ability::AbilityOpts(/*cooldown_sec*/ 0),
          lib::api::abilities::kKeyW, 0, -5);

  std::unique_ptr<lib::api::objects::Object> player = std::make_unique<
      lib::api::objects::PlayerControllableObject>(
      lib::api::objects::Object::Kind::PLAYER,
      lib::api::objects::PlayerControllableObject::PlayerControllableObjectOpts(
          true, true, 0, 0),
      absl::flat_hash_map<lib::api::objects::Object::Kind,
                          lib::api::objects::Object::CollisionCallback>(),
      std::make_pair(250, 250), 20);

  std::list<std::unique_ptr<lib::api::abilities::Ability>> abilities;
  abilities.push_back(std::move(ability_a));
  abilities.push_back(std::move(ability_d));
  abilities.push_back(std::move(ability_s));
  abilities.push_back(std::move(ability_w));
  std::unique_ptr<lib::api::Level> level = std::make_unique<lib::api::Level>();
  level->add_object_and_abilities(std::move(player), std::move(abilities));
  std::list<std::unique_ptr<lib::api::Level>> levels;
  levels.push_back(std::move(level));

  lib::api::Game& game = lib::api::Game::Create(std::move(levels));
  game.Run();
}
