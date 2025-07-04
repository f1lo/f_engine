#include "lib/api/title_screen_level.h"

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
