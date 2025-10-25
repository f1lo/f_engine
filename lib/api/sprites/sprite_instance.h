#ifndef LIB_API_SPRITES_SPRITE_INSTANCE_H
#define LIB_API_SPRITES_SPRITE_INSTANCE_H

#include <memory>
#include <optional>

#include "absl/time/time.h"
#include "lib/api/common_types.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

class SpriteInstance {
 public:
  SpriteInstance(std::shared_ptr<Sprite> sprite);
  SpriteInstance(std::shared_ptr<Sprite> sprite,
                 const absl::Duration advance_to_next_frame_after);

  void Draw(const WorldPosition draw_destination);
  void RotateAndDraw(const WorldPosition draw_destination,
                     const int rotation_degree);
  void Reset();

 private:
  void DrawInternal(const WorldPosition draw_destination,
                    const int rotation_degree);

  std::shared_ptr<Sprite> sprite_;
  const absl::Duration advance_to_next_frame_after_;
  int current_frame_to_draw_;
  std::optional<absl::Time> first_time_current_frame_was_drawn_;
  const bool is_animation_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_SPRITE_INSTANCE_H
