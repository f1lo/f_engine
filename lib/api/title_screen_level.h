#ifndef LIB_API_TITLE_SCREEN_LEVEL_H
#define LIB_API_TITLE_SCREEN_LEVEL_H

#include "lib/api/level.h"

#include "lib/api/abilities/click_ability.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

class TitleScreenLevel : public Level {
 public:
  TitleScreenLevel(const LevelId id,
                   std::unique_ptr<objects::StaticObject> start_button,
                   std::unique_ptr<objects::StaticObject> exit_button)
      : Level(id),
        start_button_(start_button.get()),
        exit_button_(exit_button.get()) {
    objects_.emplace_back(std::move(start_button));
    objects_.emplace_back(std::move(exit_button));
    std::list<std::unique_ptr<abilities::Ability>> start_ability;
    start_ability.emplace_back(std::make_unique<abilities::ClickAbility>());
    abilities_.push_back(std::move(start_ability));
    std::list<std::unique_ptr<abilities::Ability>> exit_ability;
    exit_ability.emplace_back(std::make_unique<abilities::ClickAbility>());
    abilities_.push_back(std::move(exit_ability));
  }
  ~TitleScreenLevel() override = default;

  [[nodiscard]] LevelId MaybeChangeLevel() const override;

 private:
  const objects::StaticObject* start_button_;
  const objects::StaticObject* exit_button_;
};

}  // namespace api
}  // namespace lib

#endif  //LIB_API_TITLE_SCREEN_LEVEL_H
