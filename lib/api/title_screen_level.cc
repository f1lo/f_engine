#include "lib/api/title_screen_level.h"

#include "abilities/click_ability.h"

namespace lib {
namespace api {

LevelId TitleScreenLevel::MaybeChangeLevel() const {
  if (start_button_->clicked()) {
    return kFirstLevel;
  }
  if (exit_button_->clicked()) {
    return kExitLevel;
  }

  return id();
}

}  // namespace api
}  // namespace lib
