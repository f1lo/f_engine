#ifndef LIB_API_ABILITIES_CLICK_ABILITY_H
#define LIB_API_ABILITIES_CLICK_ABILITY_H

#include <list>
#include <memory>

#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/controls.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {

class ClickAbility : public Ability {
 public:
  explicit ClickAbility(std::unique_ptr<const ControlsInterface> controls)
      : Ability(std::move(controls), {.cooldown_sec = 0}) {}
  ~ClickAbility() override = default;

  std::list<ObjectAndAbilities> Use(const AbilityContext& ctx) override;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_CLICK_ABILITY_H
