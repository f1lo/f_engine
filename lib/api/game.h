#ifndef LIB_API_GAME_H
#define LIB_API_GAME_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/log/check.h"
#include "lib/api/factories.h"
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

    static bool init_window = [opts]() {
      unsigned int flags = FLAG_VSYNC_HINT;
      if (opts.full_screen) {
        flags |= FLAG_WINDOW_UNDECORATED;
      }
      SetConfigFlags(flags);
      InitWindow(opts.screen_width, opts.screen_height, opts.title.c_str());

      if (opts.full_screen) {
        SetWindowPosition(0, 0);
      }

      return IsWindowReady();
    }();
    QCHECK(init_window);

    Game::screen_width_ = GetScreenWidth();
    Game::screen_height_ = GetScreenHeight();

    static Game game(opts.native_screen_width, opts.native_screen_height);
    return game;
  }
  ~Game();

  Game(const Game& other) = delete;
  Game& operator=(const Game&) = delete;

  void Run();

  int screen_width() { return screen_width_; }
  int screen_height() { return screen_height_; }
  int native_screen_width() { return native_screen_width_; }
  int native_screen_height() { return native_screen_height_; }
  void AddLevel(std::unique_ptr<Level> level) {
    CHECK(levels_.find(level->id()) == levels_.end())
        << "Level " << level->id() << " already exists.";
    levels_[level->id()] = std::move(level);
  }
  void set_debug_mode(const bool debug_mode) { debug_mode_ = debug_mode; }
  Factories& factories() { return factories_; }

 private:
  Game(const float native_screen_width, const float native_screen_height)
      : native_screen_width_(native_screen_width),
        native_screen_height_(native_screen_height),
        factories_({sprites::SpriteFactory(native_screen_width_,
                                           native_screen_height_),
                    objects::ObjectTypeFactory()}) {}

  absl::flat_hash_map<LevelId, std::unique_ptr<Level>> levels_;
  Stats stats_ = Stats();
  bool debug_mode_ = false;
  static inline int screen_width_ = 0;
  static inline int screen_height_ = 0;
  const int native_screen_width_;
  const int native_screen_height_;

  Factories factories_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_GAME_H
