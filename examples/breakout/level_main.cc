#include "level_main.h"

#include "lib/api/controls.h"
#include "lib/api/level.h"

namespace breakout {

using lib::api::kExitLevel;
using lib::api::kKeyEscape;
using lib::api::Level;
using lib::api::LevelId;

LevelId LevelMain::MaybeChangeLevel() const {
  if (controls_->IsPressed(kKeyEscape) || ball_->has_touched_bottom()) {
    return kExitLevel;
  }

  return id();
}

}  // namespace breakout
