#include <memory>

#include "lib/api/game.h"
#include "lib/api/level.h"

using lib::api::Game;
using lib::api::kTitleScreenLevel;
using lib::api::Level;
using lib::api::LevelBuilder;

int main() {
  Game& game =
      Game::Create(/*width=*/1500, /*height=*/1000, "Background Texture",
                   /*full_screen=*/false);
  LevelBuilder level_builder = LevelBuilder<Level>(/*id=*/kTitleScreenLevel);
  std::unique_ptr<Level> level = level_builder.Build();

  game.add_level(std::move(level));
  game.Run();
  return 0;
}