#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "g_1/levels.h"
#include "lib/api/game.h"

ABSL_FLAG(bool, is_debug_mode, false, "Run game in debug mode.");
ABSL_FLAG(bool, is_full_screen, false, "Run game in full screen.");
ABSL_FLAG(int, native_screen_width, 1920, "");
ABSL_FLAG(int, native_screen_height, 1080, "");

int main(const int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);

  lib::api::Game& game = lib::api::Game::Create({
      .native_screen_width = absl::GetFlag(FLAGS_native_screen_width),
      .native_screen_height = absl::GetFlag(FLAGS_native_screen_height),
      .screen_width = 1500,
      .screen_height = 1000,
      .full_screen = absl::GetFlag(FLAGS_is_full_screen),
      .title = "FG",
  });

  game.AddLevel(g_1::MakeTitleScreenLevel(
      game.factories(), static_cast<float>(game.native_screen_width()),
      static_cast<float>(game.native_screen_height()),
      absl::GetFlag(FLAGS_is_debug_mode)));
  game.AddLevel(g_1::MakeOpeningLevel(
      game.factories(), static_cast<float>(game.native_screen_width()),
      static_cast<float>(game.native_screen_height()),
      absl::GetFlag(FLAGS_is_debug_mode)));

  game.Run();
  return 0;
}
