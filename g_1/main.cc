#include <iostream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "g_1/levels.h"
#include "lib/api/game.h"

ABSL_FLAG(bool, is_debug_mode, false, "Run game in debug mode.");

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);

  lib::api::Game& game =
      lib::api::Game::Create(/*width=*/1500, /*height=*/1000, "g_1",
                             /*full_screen=*/false);

  game.add_level(g_1::MakeTitleScreenLevel(game.screen_width(),
                                           game.screen_height(),
                                           absl::GetFlag(FLAGS_is_debug_mode)));
  game.add_level(g_1::MakeOpeningLevel(game.screen_width(),
                                       game.screen_height(),
                                       absl::GetFlag(FLAGS_is_debug_mode)));

  game.Run();
  return 0;
}