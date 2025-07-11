#include "raylib/include/raylib.h"

#include "absl/log/check.h"
#include "lib/api/game.h"

namespace lib {
namespace api {

void Game::Run() const {
  SetTargetFPS(90);
  LevelId current_level = kTitleScreenLevel;
  while (current_level != kExitLevel) {
    auto level_it = levels_.find(current_level);
    CHECK(level_it != levels_.end())
        << "Level " << current_level << " does not exist.";
    current_level = level_it->second->Run();
  }
  CloseWindow();
}

}  // namespace api
}  // namespace lib
