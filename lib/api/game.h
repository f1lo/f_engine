#ifndef LIB_API_GAME_H
#define LIB_API_GAME_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>

#include "absl/log/check.h"
#include "lib/api/level.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {

class Game {
 public:
  static Game& Create(std::list<std::unique_ptr<Level>> levels) {
    static bool init_window = []() {
      InitWindow(1000, 1000, "abc");
      return IsWindowReady();
    }();
    QCHECK(init_window);

    static Game game(std::move(levels));
    return game;
  }

  Game(const Game& other) = delete;

  void Run();

  int screen_width() const { return GetScreenWidth(); }
  int screen_height() const { return GetScreenHeight(); }

 private:
  explicit Game(std::list<std::unique_ptr<Level>> levels)
      : levels_(std::move(levels)) {}

  std::list<std::unique_ptr<Level>> levels_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_GAME_H
