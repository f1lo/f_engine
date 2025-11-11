#include "raylib/include/raylib.h"

#include "lib/api/sprites/sprite_factory.h"

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "absl/time/time.h"
#include "lib/api/sprites/animated_sprite.h"
#include "lib/api/sprites/sprite.h"
#include "lib/api/sprites/sprite_instance.h"
#include "lib/api/sprites/static_sprite.h"

namespace lib {
namespace api {
namespace sprites {

std::unique_ptr<SpriteInstance> SpriteFactory::MakeStaticSprite(
    std::string_view resource_path) {
  std::unique_ptr<Sprite> sprite;
  auto [sprite_it, inserted] = sprites_.try_emplace(resource_path, nullptr);
  if (inserted) {
    sprite_it->second =
        absl::WrapUnique(new StaticSprite(std::string(resource_path)));
  }

  std::unique_ptr<SpriteInstance> sprite_instance =
      absl::WrapUnique(new SpriteInstance(sprite_it->second.get()));

  return sprite_instance;
}

std::unique_ptr<SpriteInstance> SpriteFactory::MakeAnimatedSprite(
    std::string_view resource_path, const int frame_count,
    const absl::Duration advance_to_next_frame_after) {
  std::unique_ptr<Sprite> sprite;
  auto [sprite_it, inserted] = sprites_.try_emplace(resource_path, nullptr);
  if (inserted) {
    sprite_it->second = absl::WrapUnique(
        new AnimatedSprite(std::string(resource_path), frame_count));
  }

  std::unique_ptr<SpriteInstance> sprite_instance = absl::WrapUnique(
      new SpriteInstance(sprite_it->second.get(), advance_to_next_frame_after));

  return sprite_instance;
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
