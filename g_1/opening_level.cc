#include "g_1/opening_level.h"

#include "lib/api/abilities/ability.h"
#include "lib/api/controls.h"
#include "lib/api/level.h"

namespace g_1 {

using lib::api::kKeyEscape;
using lib::api::kTitleScreenLevel;
using lib::api::Level;
using lib::api::LevelId;

LevelId OpeningLevel::MaybeChangeLevel() const {
  if (controls_->IsPressed(kKeyEscape)) {
    return kTitleScreenLevel;
  }
  return id();
}
}  // namespace g_1