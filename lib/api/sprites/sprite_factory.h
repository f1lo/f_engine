#ifndef LIB_API_SPRITES_SPRITE_FACTORY_H
#define LIB_API_SPRITES_SPRITE_FACTORY_H

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/memory/memory.h"
#include "absl/time/time.h"
#include "lib/api/sprites/sprite.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {

class Game;

namespace sprites {

class SpriteFactory {
 public:
  std::unique_ptr<SpriteInstance> MakeStaticSprite(
      std::string_view resource_path);
  std::unique_ptr<SpriteInstance> MakeAnimatedSprite(
      std::string_view resource_path, const int frame_count,
      const absl::Duration advance_to_next_frame_after);

 private:
  friend class lib::api::Game;
  SpriteFactory() = default;

  absl::flat_hash_map<std::string, std::unique_ptr<Sprite>> sprites_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_SPRITE_FACTORY_H
