#ifndef LIB_API_ABILITIES_PROJECTILE_ABILITY_H
#define LIB_API_ABILITIES_PROJECTILE_ABILITY_H

#include <list>

#include "absl/container/flat_hash_set.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/projectile_object.h"

namespace lib {
namespace api {
namespace abilities {

class ProjectileAbility : public Ability {
 public:
  struct ProjectileAbilityOpts : AbilityOpts {
    ProjectileAbilityOpts(const uint32_t cooldown_sec)
        : AbilityOpts(cooldown_sec) {}
  };

  explicit ProjectileAbility(
      const Kind projectile_kind, const ProjectileAbilityOpts& opts,
      lib::api::objects::ProjectileObject::ProjectileObjectOpts
          projectile_object_opts)
      : Ability(opts),
        projectile_kind_(projectile_kind),
        projectile_ability_opts_(opts),
        projectile_object_opts_(std::move(projectile_object_opts)) {}

  std::list<ObjectAndAbilities> Use(const Camera& camera) override;

 private:
  Kind projectile_kind_;
  ProjectileAbilityOpts projectile_ability_opts_;
  lib::api::objects::ProjectileObject::ProjectileObjectOpts
      projectile_object_opts_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  //LIB_API_ABILITIES_PROJECTILE_ABILITY_H
