#include "raylib/include/raylib.h"

#include "lib/api/abilities/click_ability.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {

using objects::Object;
using objects::StaticObject;

void ClickAbility::MaybeUseModifyUser(Object& user) {
  if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    user.set_clicked(false);
    return;
  }
  if (user.CollidesWith(StaticObject(
          /*kind=*/kMousePointer,
          /*options=*/
          /*options=*/
          {/*is_hit_box_active=*/true,
           /*should_draw_hit_box=*/false},
          /*hit_box_vertices=*/{{GetMouseX(), GetMouseY()}}))) {
    user.set_clicked(true);
  }
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
