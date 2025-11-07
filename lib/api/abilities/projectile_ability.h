#ifndef LIB_API_ABILITIES_PROJECTILE_ABILITY_H
#define LIB_API_ABILITIES_PROJECTILE_ABILITY_H

#include <list>
#include <memory>

#include "absl/container/flat_hash_set.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/controls.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/projectile_object.h"

namespace lib {
namespace api {
namespace abilities {

class ProjectileAbility : public Ability {
 public:
  struct ProjectileAbilityOpts {
    uint32_t cooldown_sec;
  };

  ProjectileAbility(std::unique_ptr<const ControlsInterface> controls,
                    const lib::api::objects::Kind projectile_kind,
                    const ProjectileAbilityOpts& opts,
                    lib::api::objects::ProjectileObject::ProjectileObjectOpts
                        projectile_object_opts)
      : Ability(std::move(controls), {.cooldown_sec = opts.cooldown_sec}),
        projectile_kind_(projectile_kind),
        projectile_object_opts_(projectile_object_opts) {}

  std::list<ObjectAndAbilities> Use(const Camera& camera) override;

 private:
  lib::api::objects::Kind projectile_kind_;
  lib::api::objects::ProjectileObject::ProjectileObjectOpts
      projectile_object_opts_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  //LIB_API_ABILITIES_PROJECTILE_ABILITY_H
