#include "lib/api/abilities/click_ability.h"

#include <list>
#include <memory>

#include "lib/api/abilities/controls.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {

using api::Camera;
using lib::api::objects::kMousePointer;
using objects::Object;
using objects::StaticObject;

std::list<ObjectAndAbilities> ClickAbility::Use(const Camera& camera) {
  if (!controls_->IsPrimaryPressed()) {
    user()->set_clicked(false);
    return {};
  }
  const WorldPosition cursor_pos_world =
      camera.GetWorldPosition(controls_->GetCursorPos());
  if (user()->CollidesWith(StaticObject(
          /*kind=*/kMousePointer,
          /*options=*/
          {/*is_hit_box_active=*/true,
           /*should_draw_hit_box=*/false},
          /*hit_box_vertices=*/{{cursor_pos_world.x, cursor_pos_world.y}}))) {
    user()->set_clicked(true);
  }
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
