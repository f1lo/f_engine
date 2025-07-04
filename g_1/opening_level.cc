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

}  // namespace g_1