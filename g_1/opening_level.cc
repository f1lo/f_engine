#include "g_1/opening_level.h"

#include "lib/api/abilities/keys.h"
#include "lib/api/level.h"

namespace g_1 {

using lib::api::kTitleScreenLevel;
using lib::api::Level;
using lib::api::LevelId;
using lib::api::abilities::IsPressed;
using lib::api::abilities::kKeyEscape;

LevelId OpeningLevel::MaybeChangeLevel() const {
  if (IsPressed(kKeyEscape)) {
    return kTitleScreenLevel;
  }
  return id();
}

OpeningLevelBuilder& OpeningLevelBuilder::AddPlayerAndAbilities(
    std::unique_ptr<Player> player,
    std::list<std::unique_ptr<lib::api::abilities::Ability>> abilities) {
  // Unsafe.
  level_->player_ = player.get();
  AddObjectAndAbilities(std::move(player), std::move(abilities),
                        /*attach_camera=*/true);
  return *this;
}

}  // namespace g_1