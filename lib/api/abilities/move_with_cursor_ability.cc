#include "lib/api/abilities/move_with_cursor_ability.h"

#include <list>
#include <memory>

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/abilities/keys.h"
#include "lib/api/camera.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {

using api::Camera;
using lib::api::objects::kMousePointer;
using objects::MovableObject;
using objects::Object;

std::list<ObjectAndAbilities> MoveWithCursorAbility::Use(const Camera& camera) {
  MovableObject* const movable_user = dynamic_cast<MovableObject*>(user_);
  CHECK(movable_user);
  if (cursor_last_clicked_pos_.has_value()) {
    if (movable_user->CollidesWith(objects::StaticObject(
            /*kind=*/kMousePointer,
            /*options=*/
            {/*is_hit_box_active=*/true,
             /*should_draw_hit_box=*/false},
            /*hit_box_vertices=*/
            {{cursor_last_clicked_pos_->x, cursor_last_clicked_pos_->y}}))) {
      cursor_last_clicked_pos_ = std::nullopt;
      movable_user->freeze_until_next_set_direction();
      return {};
    }
  }
  if (!IsSecondaryPressed()) {
    return {};
  }
  const WorldPosition cursor_pos_world =
      camera.GetWorldPosition(GetCursorPos());
  cursor_last_clicked_pos_ = cursor_pos_world;
  movable_user->SetDirectionRelative(cursor_pos_world.x, cursor_pos_world.y);
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
