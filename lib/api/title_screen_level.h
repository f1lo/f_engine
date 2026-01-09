#ifndef LIB_API_TITLE_SCREEN_LEVEL_H
#define LIB_API_TITLE_SCREEN_LEVEL_H

#include <memory>

#include "absl/log/check.h"
#include "lib/api/level.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

class TitleScreenLevel : public Level {
 public:
  ~TitleScreenLevel() override = default;

  [[nodiscard]] LevelId MaybeChangeLevel() const override;

 private:
  friend class TitleScreenLevelBuilder;
  friend class LevelBuilder<TitleScreenLevel>;

  // Does not take ownership.
  const objects::StaticObject* start_button_;
  const objects::StaticObject* exit_button_;

 protected:
  TitleScreenLevel(const LevelId id, const float native_screen_width,
                   const float native_screen_height)
      : Level(id, native_screen_width, native_screen_height),
        start_button_(nullptr),
        exit_button_(nullptr) {}
};

class TitleScreenLevelBuilder : LevelBuilder<TitleScreenLevel> {
 public:
  TitleScreenLevelBuilder(const float native_screen_width,
                          const float native_screen_height)
      : LevelBuilder(kTitleScreenLevel, native_screen_width,
                     native_screen_height) {}

  TitleScreenLevelBuilder& AddStartButton(
      std::unique_ptr<objects::StaticObject> start_button) {
    CHECK(level_->start_button_ == nullptr)
        << "Cannot add start button more than once.";
    level_->start_button_ = start_button.get();

    std::list<std::unique_ptr<abilities::Ability>> start_ability;
    AddObject(std::move(start_button));

    return *this;
  }

  TitleScreenLevelBuilder& AddExitButton(
      std::unique_ptr<objects::StaticObject> exit_button) {
    CHECK(level_->exit_button_ == nullptr)
        << "Cannot add exit button more than once.";
    level_->exit_button_ = exit_button.get();

    std::list<std::unique_ptr<abilities::Ability>> start_ability;
    AddObject(std::move(exit_button));

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

#endif  // LIB_API_TITLE_SCREEN_LEVEL_H
