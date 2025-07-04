#ifndef G_1_OPENING_LEVEL_H
#define G_1_OPENING_LEVEL_H

#include "lib/api/level.h"

namespace g_1 {

class OpeningLevel : public lib::api::Level {
 public:
  explicit OpeningLevel(const lib::api::LevelId id) : Level(id) {}
  lib::api::LevelId MaybeChangeLevel() const override;
};

}  // namespace g_1

#endif  // G_1_OPENING_LEVEL_H
