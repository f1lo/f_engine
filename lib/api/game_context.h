#ifndef LIB_API_GAME_CONTEXT_H
#define LIB_API_GAME_CONTEXT_H

#include "raylib/include/raylib.h"

#include <memory>
#include <vector>

#include "absl/log/check.h"
#include "game_context.h"
#include "lib/internal/object.h"

namespace lib {
namespace api {

class GameContext {
public:
  static GameContext &Create() {
    static bool init_window = []() {
      InitWindow(1200, 2000, "");
      return IsWindowReady();
    }();
    QCHECK(init_window);

    static GameContext context;
    return context;
  }

  GameContext(const GameContext &other) = delete;

  void RunGame();
  void AddObject(std::unique_ptr<internal::Object> object);

private:
  explicit GameContext() :
      objects_(std::vector<std::unique_ptr<internal::Object>>{}) {}
  void DrawFrame();

  std::vector<std::unique_ptr<internal::Object>> objects_;
};

} // namespace api
} // namespace lib

#endif // LIB_API_GAME_CONTEXT_H
