#include "raylib/include/raylib.h"

#include <iostream>
#include <string>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/any_invocable.h"
#include "absl/strings/str_join.h"

int main() {
  std::cout << "XXXXXXXXXXXXXXX\n";
  InitWindow(1500, 1000, "window");
  while (!WindowShouldClose()) {
    BeginDrawing();
    DrawRectangleLines(1205, 130, 1335 - 1205, 30, RED);
    DrawCircle(1443, 179, 20, RED);
    EndDrawing();
  }
}
