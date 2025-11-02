#include "level_main.h"

#include "lib/api/abilities/controls.h"
#include "lib/api/level.h"

namespace breakout {

using lib::api::kExitLevel;
using lib::api::Level;
using lib::api::LevelId;
using lib::api::abilities::kKeyEscape;

LevelId LevelMain::MaybeChangeLevel() const {
  if (controls_->IsPressed(kKeyEscape) || ball_->has_touched_bottom()) {
    return kExitLevel;
  }

  return id();
}

}  // namespace breakout
