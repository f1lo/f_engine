#include <iostream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "g_1/levels.h"
#include "lib/api/game.h"

ABSL_FLAG(bool, is_debug_mode, false, "Run game in debug mode.");
ABSL_FLAG(bool, is_full_screen, false, "Run game in full screen.");

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);

  lib::api::Game& game = lib::api::Game::Create(
      /*width=*/1500, /*height=*/1000, "g_1",
      /*full_screen=*/absl::GetFlag(FLAGS_is_full_screen));

  game.add_level(g_1::MakeTitleScreenLevel(absl::GetFlag(FLAGS_is_debug_mode)));
  game.add_level(g_1::MakeOpeningLevel(absl::GetFlag(FLAGS_is_debug_mode)));

  game.Run();
  return 0;
}