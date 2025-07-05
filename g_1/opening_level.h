#ifndef G_1_OPENING_LEVEL_H
#define G_1_OPENING_LEVEL_H

#include "g_1/player.h"
#include "lib/api/level.h"

namespace g_1 {

class OpeningLevel : public lib::api::Level {
 public:
  explicit OpeningLevel(const lib::api::LevelId id)
      : Level(id), player_(nullptr) {}
  lib::api::LevelId MaybeChangeLevel() const override;

 protected:
  friend class OpeningLevelBuilder;
  const Player* player_;
};

class OpeningLevelBuilder : public lib::api::LevelBuilder<OpeningLevel> {
 public:
  explicit OpeningLevelBuilder(const lib::api::LevelId id) : LevelBuilder(id) {}

  OpeningLevelBuilder& SetPlayer(const Player* player) {
    level_->player_ = player;
    return *this;
  }
};

}  // namespace g_1

#endif  // G_1_OPENING_LEVEL_H
