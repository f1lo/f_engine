#ifndef G_1_LEVELS_H
#define G_1_LEVELS_H

#include <memory>

#include "lib/api/factories.h"
#include "lib/api/level.h"

namespace g_1 {

std::unique_ptr<lib::api::Level> MakeTitleScreenLevel(
    lib::api::Factories& factories, float native_screen_width,
    float native_screen_height, bool debug_mode);
std::unique_ptr<lib::api::Level> MakeOpeningLevel(
    lib::api::Factories& factories, float native_screen_width,
    float native_screen_height, bool debug_mode);

}  // namespace g_1

#endif  // G_1_LEVELS_H
