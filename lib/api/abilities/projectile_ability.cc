#include "lib/api/abilities/projectile_ability.h"

#include "lib/api/abilities/ability.h"
#include "lib/api/camera.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {

std::list<ObjectAndAbilities> ProjectileAbility::Use(const Camera& camera) {
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
