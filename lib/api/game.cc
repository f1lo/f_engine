#include "raylib/include/raylib.h"

#include "lib/api/game.h"

namespace lib {
namespace api {

void Game::Run() {
  // TODO(f1lo): Implement more levels.
  SetTargetFPS(90);
  levels_.begin()->get()->Run();
  CloseWindow();
}

}  // namespace api
}  // namespace lib
