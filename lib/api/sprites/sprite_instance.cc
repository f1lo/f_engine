#include "lib/api/sprites/sprite_instance.h"

#include <memory>

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "lib/api/sprites/sprite.h"

namespace lib {
namespace api {
namespace sprites {

SpriteInstance::SpriteInstance(std::shared_ptr<Sprite> sprite)
    : sprite_(std::move(sprite)),
      advance_to_next_frame_after_(absl::ZeroDuration()),
      current_frame_to_draw_(0),
      first_time_current_frame_was_drawn_(std::nullopt),
      is_animation_(false) {}

SpriteInstance::SpriteInstance(std::shared_ptr<Sprite> sprite,
                               const absl::Duration advance_to_next_frame_after)
    : sprite_(std::move(sprite)),
      advance_to_next_frame_after_(advance_to_next_frame_after),
      current_frame_to_draw_(0),
      first_time_current_frame_was_drawn_(std::nullopt),
      is_animation_(sprite_->total_frames() != 1) {}

void SpriteInstance::DrawInternal(const WorldPosition draw_destination,
                                  const int rotation_degree) {
  if (!is_animation_) {
    sprite_->RotateAndDraw(draw_destination, rotation_degree);
  }

  const absl::Time now = absl::Now();
  if (!first_time_current_frame_was_drawn_.has_value()) {
    first_time_current_frame_was_drawn_ = now;
  }

  if (now - *first_time_current_frame_was_drawn_ >=
      advance_to_next_frame_after_) {
    current_frame_to_draw_++;
    current_frame_to_draw_ %= sprite_->total_frames();
    first_time_current_frame_was_drawn_ = now;
  }

  sprite_->RotateAndDraw(draw_destination, rotation_degree,
                         current_frame_to_draw_);
}

void SpriteInstance::Draw(const WorldPosition draw_destination) {
  DrawInternal(draw_destination, /*rotation_degree=*/0);
}

void SpriteInstance::RotateAndDraw(const WorldPosition draw_destination,
                                   const int rotation_degree) {
  DrawInternal(draw_destination, rotation_degree);
}

void SpriteInstance::Reset() {
  first_time_current_frame_was_drawn_ = std::nullopt;
  current_frame_to_draw_ = 0;
}

int SpriteInstance::MainSpriteHeight() const {
  return sprite_->sprite_height();
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
