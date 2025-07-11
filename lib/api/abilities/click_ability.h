#ifndef LIB_API_ABILITIES_CLICK_ABILITY_H
#define LIB_API_ABILITIES_CLICK_ABILITY_H

#include "lib/api/abilities/ability.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {

class ClickAbility : public Ability {
 public:
  ClickAbility() : Ability({.cooldown_sec = 0}) {}

  void MaybeUseModifyUser(objects::Object& user) override;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_CLICK_ABILITY_H
