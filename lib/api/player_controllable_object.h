#ifndef LIB_API_PLAYER_CONTROLLABLE_OBJECT_H
#define LIB_API_PLAYER_CONTROLLABLE_OBJECT_H

#include "lib/internal/object.h"

#include <vector>

#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
class PlayerControllableObject final : public internal::Object {
public:
  struct ControllableObjectCreationOpts {
    bool is_hit_box_active;
    bool should_draw_hit_box;
  };

  PlayerControllableObject(
      const std::vector<std::pair<int, int>> &hit_box_vertices,
      ControllableObjectCreationOpts opts);
  PlayerControllableObject(std::pair<int, int> hit_box_center,
                           uint32_t hit_box_radius,
                           ControllableObjectCreationOpts opts);

  void Move(int pos_x, int pos_y) override;

private:
  // Assumes `BeginDrawing` has been called.
  void Draw() override;

  bool is_hit_box_active_;
  bool should_draw_hit_box_;

  internal::HitBox hit_box_;
};
} // namespace api
} // namespace lib

#endif // LIB_API_PLAYER_CONTROLLABLE_OBJECT_H
