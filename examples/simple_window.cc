#include "raylib/include/raylib.h"

#include <iostream>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/any_invocable.h"
#include "absl/strings/str_join.h"

int main() {
  static bool init_window = []() {
    InitWindow(1000, 1000, "abc");
    return IsWindowReady();
  }();
  std::cout << init_window << std::endl;

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawCircleLines(250, 250, /*radius*/ static_cast<float>(20), RED);
    EndDrawing();
  }

  CloseWindow();
}
