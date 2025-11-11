#ifndef G_1_LEVELS_H
#define G_1_LEVELS_H

#include <memory>

#include "lib/api/level.h"
#include "lib/api/sprites/sprite_factory.h"

namespace g_1 {

std::unique_ptr<lib::api::Level> MakeTitleScreenLevel(
    lib::api::sprites::SpriteFactory& sprite_factory, bool debug_mode);
std::unique_ptr<lib::api::Level> MakeOpeningLevel(
    lib::api::sprites::SpriteFactory& sprite_factory, bool debug_mode);

}  // namespace g_1

#endif  // G_1_LEVELS_H
