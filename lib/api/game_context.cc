#include "raylib/include/raylib.h"

#include "game_context.h"

#include "lib/internal/object.h"

namespace lib {
namespace api {

void GameContext::AddObject(std::unique_ptr<internal::Object> object) {
  objects_.push_back(std::move(object));
}

void GameContext::DrawFrame() {
  BeginDrawing();
  for (const auto &object: objects_) {
    object->Draw();
  }
  EndDrawing();
}

void GameContext::RunGame() {
  while (!WindowShouldClose()) {
    DrawFrame();
  }

  CloseWindow();
}


} // namespace api
} // namespace lib
