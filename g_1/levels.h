#ifndef G_1_LEVELS_H
#define G_1_LEVELS_H

#include <memory>

#include "lib/api/level.h"

namespace g_1 {

std::unique_ptr<lib::api::Level> MakeTitleScreenLevel(int screen_width,
                                                      int screen_height,
                                                      bool debug_mode);
std::unique_ptr<lib::api::Level> MakeOpeningLevel(int screen_width,
                                                  int screen_height,
                                                  bool debug_mode);

}  // namespace g_1

#endif  // G_1_LEVELS_H
