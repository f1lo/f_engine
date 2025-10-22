#ifndef LIB_API_GAME_H
#define LIB_API_GAME_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/log/check.h"
#include "lib/api/level.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {

class Game {
 public:
  static Game& Create(int width, int height, const std::string& title,
                      bool full_screen) {
    if (full_screen) {
      width = GetScreenWidth();
      height = GetScreenHeight();
    }
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

  void Run() const;

  int screen_width() const { return GetScreenWidth(); }
  int screen_height() const { return GetScreenHeight(); }
  void AddLevel(std::unique_ptr<Level> level) {
    CHECK(levels_.find(level->id()) == levels_.end())
        << "Level " << level->id() << " already exists.";
    levels_[level->id()] = std::move(level);
  }
  void set_debug_mode(const bool debug_mode) { debug_mode_ = debug_mode; }

 private:
  explicit Game() = default;

  absl::flat_hash_map<LevelId, std::unique_ptr<Level>> levels_;
  bool debug_mode_ = false;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_GAME_H
