#include "raylib/include/raylib.h"

#include "lib/api/abilities/click_ability.h"

#include <list>
#include <memory>

#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {
namespace abilities {

using objects::Object;
using objects::StaticObject;

std::list<std::unique_ptr<Object>> ClickAbility::Use() {
  if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    user_->set_clicked(false);
    return {};
  }
  if (user_->CollidesWith(StaticObject(
          /*kind=*/kMousePointer,
          /*options=*/
          /*options=*/
          {/*is_hit_box_active=*/true,
           /*should_draw_hit_box=*/false},
          /*hit_box_vertices=*/{{GetMouseX(), GetMouseY()}}))) {
    user_->set_clicked(true);
  }
  return {};
}

}  // namespace abilities
}  // namespace api
}  // namespace lib
