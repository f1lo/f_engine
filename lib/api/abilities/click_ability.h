#ifndef LIB_API_ABILITIES_CLICK_ABILITY_H
#define LIB_API_ABILITIES_CLICK_ABILITY_H

#include <list>
#include <memory>

#include "lib/api/abilities/ability.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {

class ClickAbility : public Ability {
 public:
  ClickAbility() : Ability({.cooldown_sec = 0}) {}

  std::list<std::unique_ptr<objects::Object>> Use() override;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_CLICK_ABILITY_H
