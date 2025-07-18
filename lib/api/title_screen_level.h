#ifndef LIB_API_TITLE_SCREEN_LEVEL_H
#define LIB_API_TITLE_SCREEN_LEVEL_H

#include "abilities/click_ability.h"
#include "absl/log/check.h"
#include "lib/api/level.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

class TitleScreenLevel : public Level {
 public:
  explicit TitleScreenLevel(const LevelId id)
      : Level(id), start_button_(nullptr), exit_button_(nullptr) {}
  ~TitleScreenLevel() override = default;

  [[nodiscard]] LevelId MaybeChangeLevel() const override;

 private:
  friend class TitleScreenLevelBuilder;

  // Does not take ownership.
  const objects::StaticObject* start_button_;
  const objects::StaticObject* exit_button_;
};

class TitleScreenLevelBuilder : LevelBuilder<TitleScreenLevel> {
 public:
  explicit TitleScreenLevelBuilder() : LevelBuilder(kTitleScreenLevel) {}

  TitleScreenLevelBuilder& AddStartButton(
      std::unique_ptr<objects::StaticObject> start_button) {
    level_->start_button_ = start_button.get();

    std::list<std::unique_ptr<abilities::Ability>> start_ability;
    start_ability.emplace_back(std::make_unique<abilities::ClickAbility>());
    AddObjectAndAbilities(std::move(start_button), std::move(start_ability));

    return *this;
  }

  TitleScreenLevelBuilder& AddExitButton(
      std::unique_ptr<objects::StaticObject> exit_button) {
    level_->exit_button_ = exit_button.get();

    std::list<std::unique_ptr<abilities::Ability>> start_ability;
    start_ability.emplace_back(std::make_unique<abilities::ClickAbility>());
    AddObjectAndAbilities(std::move(exit_button), std::move(start_ability));

    return *this;
  }

  std::unique_ptr<TitleScreenLevel> Build() override {
    CHECK(level_->start_button_ != nullptr) << "Start button not added.";
    CHECK(level_->exit_button_ != nullptr) << "Exit button not added.";

    return std::move(level_);
  }
};

}  // namespace api
}  // namespace lib

#endif  //LIB_API_TITLE_SCREEN_LEVEL_H
