#ifndef LIB_API_ABILITIES_PROJECTILE_ABILITY_H
#define LIB_API_ABILITIES_PROJECTILE_ABILITY_H

#include <list>

#include "lib/api/abilities/ability.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {

class ProjectileAbility : public Ability {
 public:
  struct ProjectileAbilityOpts : AbilityOpts {
    ProjectileAbilityOpts(const uint32_t cooldown_sec, const double velocity,
                          const bool despawn_outside_screen_area)
        : AbilityOpts(cooldown_sec),
          velocity(velocity),
          despawn_outside_screen_area(despawn_outside_screen_area) {}
    double velocity;
    bool despawn_outside_screen_area;
  };

  explicit ProjectileAbility(const ProjectileAbilityOpts& opts)
      : Ability(opts), projectile_ability_opts_(opts) {}

  std::list<ObjectAndAbilities> Ability::Use(const Camera& camera) override;

 private:
  ProjectileAbilityOpts projectile_ability_opts_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  //LIB_API_ABILITIES_PROJECTILE_ABILITY_H
