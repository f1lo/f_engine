#include "lib/api/abilities/move_with_cursor_ability.h"

#include <list>
#include <memory>
#include <optional>

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/common_types.h"
#include "lib/api/controls.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {

using objects::MovableObject;
using objects::Object;

std::list<ObjectAndAbilities> MoveWithCursorAbility::Use(
    const AbilityContext& ctx) {
  MovableObject* const movable_user = dynamic_cast<MovableObject*>(user());
  CHECK(movable_user) << "User not movable.";
  if (cursor_last_clicked_pos_.has_value()) {
    if (movable_user->CollidesWith(objects::StaticObject(
            /*type=*/objects::ObjectTypeFactory::MakeMousePointer(),
            /*options=*/
            {/*is_hit_box_active=*/true,
             /*should_draw_hit_box=*/false},
            FPoint{cursor_last_clicked_pos_->x,
                   cursor_last_clicked_pos_->y}))) {
      cursor_last_clicked_pos_ = std::nullopt;
      movable_user->freeze_until_next_set_direction();
      return {};
    }
  }
  if (!controls_->IsSecondaryPressed()) {
    return {};
  }
  const std::optional<const WorldPosition> cursor_pos_world =
      GetMouseWorldPosition(ctx.camera, ctx.view_port_ctx, *controls_);
  if (!cursor_pos_world.has_value()) {
    return {};
  }
  cursor_last_clicked_pos_ = *cursor_pos_world;
  movable_user->SetDirectionRelative(cursor_pos_world->x, cursor_pos_world->y);
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
