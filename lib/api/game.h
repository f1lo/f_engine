#ifndef LIB_API_GAME_H
#define LIB_API_GAME_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>
#include <string>

#include "absl/log/check.h"
#include "lib/api/level.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {

class Game {
 public:
  static Game& Create(int width, int height, const std::string& title,
                      bool full_screen) {
    static bool init_window = [width, height, title, full_screen]() {
      InitWindow(width, height, title.c_str());

      if (full_screen) {
        ToggleBorderlessWindowed();
      }
      return IsWindowReady();
    }();
    QCHECK(init_window);

    static Game game;
    return game;
  }

  Game(const Game& other) = delete;

  void Run();

  int screen_width() const { return GetScreenWidth(); }
  int screen_height() const { return GetScreenHeight(); }
  void set_levels(std::list<std::unique_ptr<Level>> levels) {
    levels_ = std::move(levels);
  }

 private:
  explicit Game() = default;

  std::list<std::unique_ptr<Level>> levels_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_GAME_H
