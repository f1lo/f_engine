#include "raylib/include/raylib.h"

#include "lib/api/game.h"

namespace lib {
namespace api {

void Game::Run() const {
  // TODO(f1lo): Implement more levels.
  SetTargetFPS(90);
  if (levels_.begin()->get()->Run() == kExitLevel) {
    CloseWindow();
  }
}

}  // namespace api
}  // namespace lib
