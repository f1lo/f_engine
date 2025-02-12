#include <memory>

#include "lib/api/game_context.h"
#include "lib/api/player_controllable_object.h"

int main() {
  lib::api::GameContext &context = lib::api::GameContext::Create();
  std::unique_ptr<lib::api::PlayerControllableObject> player =
      std::make_unique<lib::api::PlayerControllableObject>(
          lib::api::PlayerControllableObject(
              {100, 100}, 50,
              {.is_hit_box_active = true, .should_draw_hit_box = true}));
  context.AddObject(std::move(player));

  context.RunGame();
}
