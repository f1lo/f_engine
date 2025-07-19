#ifndef G_1_OPENING_LEVEL_H
#define G_1_OPENING_LEVEL_H

#include "g_1/player.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/level.h"

namespace g_1 {

class OpeningLevel : public lib::api::Level {
 public:
  explicit OpeningLevel(const lib::api::LevelId id)
      : Level(id), player_(nullptr) {}
  [[nodiscard]] lib::api::LevelId MaybeChangeLevel() const override;

 protected:
  friend class OpeningLevelBuilder;
  const Player* player_;
};

class OpeningLevelBuilder : public lib::api::LevelBuilder<OpeningLevel> {
 public:
  explicit OpeningLevelBuilder(const lib::api::LevelId id) : LevelBuilder(id) {}

  OpeningLevelBuilder& AddPlayerAndAbilities(
      std::unique_ptr<Player> player,
      std::list<std::unique_ptr<lib::api::abilities::Ability>> abilities) {
    level_->player_ = player.get();
    AddObjectAndAbilities(std::move(player), std::move(abilities),
                          /*attach_camera=*/true);
    return *this;
  }
};

}  // namespace g_1

#endif  // G_1_OPENING_LEVEL_H
