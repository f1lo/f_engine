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
#include "lib/api/objects/object_type.h"
#include "lib/api/sprites/sprite_factory.h"
#include "lib/api/stats.h"

namespace lib {
namespace api {

class Game {
 public:
  struct GameOpts {
    int native_screen_width;
    int native_screen_height;
    int screen_width;
    int screen_height;
    bool full_screen;
    std::string title;
  };

  static Game& Create(GameOpts opts) {
    if (opts.full_screen) {
      opts.screen_width = GetScreenWidth();
      opts.screen_height = GetScreenHeight();
    }
    Game::screen_width_ = opts.screen_width;
    Game::screen_height_ = opts.screen_height;
    static bool init_window = [opts]() {
      InitWindow(opts.screen_width, opts.screen_height, opts.title.c_str());
      if (opts.full_screen) {
        ToggleBorderlessWindowed();
      }

      return IsWindowReady();
    }();
    QCHECK(init_window);

    static Game game;
    return game;
  }
  ~Game();

  Game(const Game& other) = delete;
  Game& operator=(const Game&) = delete;

  void Run();

  static int screen_width() { return screen_width_; }
  static int screen_height() { return screen_height_; }
  void AddLevel(std::unique_ptr<Level> level) {
    CHECK(levels_.find(level->id()) == levels_.end())
        << "Level " << level->id() << " already exists.";
    levels_[level->id()] = std::move(level);
  }
  void set_debug_mode(const bool debug_mode) { debug_mode_ = debug_mode; }
  sprites::SpriteFactory& sprite_factory() { return sprite_factory_; }
  objects::ObjectTypeFactory& object_type_factory() {
    return object_type_factory_;
  }

 private:
  Game() = default;

  absl::flat_hash_map<LevelId, std::unique_ptr<Level>> levels_;
  sprites::SpriteFactory sprite_factory_ = sprites::SpriteFactory();
  objects::ObjectTypeFactory object_type_factory_ =
      objects::ObjectTypeFactory();
  Stats stats_ = Stats();
  bool debug_mode_ = false;
  static inline int screen_width_ = 0;
  static inline int screen_height_ = 0;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_GAME_H
