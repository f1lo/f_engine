#ifndef LIB_API_ABILITIES_MOVE_WITH_CURSOR_ABILITY_H
#define LIB_API_ABILITIES_MOVE_WITH_CURSOR_ABILITY_H

#include <list>
#include <memory>
#include <optional>

#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/controls.h"
#include "lib/api/camera.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {

class MoveWithCursorAbility : public Ability {
 public:
  explicit MoveWithCursorAbility(
      std::unique_ptr<const ControlsInterface> controls)
      : Ability(std::move(controls), /*opts=*/{.cooldown_sec = 0}),
        cursor_last_clicked_pos_(std::nullopt) {}
  ~MoveWithCursorAbility() override = default;

  std::list<ObjectAndAbilities> Use(const Camera& camera) override;

 private:
  std::optional<WorldPosition> cursor_last_clicked_pos_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  //LIB_API_ABILITIES_MOVE_WITH_CURSOR_ABILITY_H
