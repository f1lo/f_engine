#include "lib/api/objects/player_controllable_object.h"

#include <list>
#include <memory>

#include "absl/container/flat_hash_map.h"
#include "lib/api/objects/object.h"
#include "lib/internal/geometry/shape.h"
#include "lib/internal/hit_box.h"
#include "movable_object.h"

namespace lib {
namespace api {
namespace objects {

using internal::HitBox;
using internal::Point;

PlayerControllableObject::PlayerControllableObject(
    const Kind kind, const PlayerControllableObjectOpts options,
    const std::vector<std::pair<int, int>>& hit_box_vertices)
    : MovableObject(
          kind,
          MovableObjectOpts(/*is_hit_box_active=*/options.is_hit_box_active,
                            /*should_draw_hit_box=*/options.should_draw_hit_box,
                            /*velocity_x=*/options.velocity_x,
                            /*velocity_y=*/options.velocity_y),
          hit_box_vertices) {}

PlayerControllableObject::PlayerControllableObject(
    const Kind kind, const PlayerControllableObjectOpts options,
    const std::pair<int, int> hit_box_center, const uint32_t hit_box_radius)
    : MovableObject(
          kind,
          MovableObjectOpts(/*is_hit_box_active=*/options.is_hit_box_active,
                            /*should_draw_hit_box=*/options.should_draw_hit_box,
                            /*velocity_x=*/options.velocity_x,
                            /*velocity_y=*/options.velocity_y),
          hit_box_center, hit_box_radius) {}

// Assumes `BeginDrawing` has been called.
void PlayerControllableObject::Draw() const {
  // TODO(f1lo): Draw texture in the future.
  if (!options().should_draw_hit_box) {
    return;
  }
  hit_box().Draw();
}

void PlayerControllableObject::Update(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  Move();
  if (UpdateInternal(other_objects)) {
    this->ResetLastMove();
  }
}

}  // namespace objects
}  // namespace api
}  // namespace lib
