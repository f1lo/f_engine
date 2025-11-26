#ifndef LIB_API_SPRITES_SPRITE_FACTORY_H
#define LIB_API_SPRITES_SPRITE_FACTORY_H

#include <memory>
#include <string>
#include <string_view>

#include "absl/container/flat_hash_map.h"
#include "absl/time/time.h"
#include "lib/api/sprites/sprite.h"
#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {

class Game;
class LevelTest;

namespace objects {
class StaticObjectTest;
}  // namespace objects

namespace sprites {

class SpriteTest;

class SpriteFactory {
 public:
  std::unique_ptr<SpriteInstance> MakeStaticSprite(
      std::string_view resource_path);
  std::unique_ptr<SpriteInstance> MakeBackgroundStaticSprite(
      std::string_view resource_path, float parallax_factor = 1);
  std::unique_ptr<SpriteInstance> MakeAnimatedSprite(
      std::string_view resource_path, int frame_count,
      absl::Duration advance_to_next_frame_after);

 private:
  friend class lib::api::Game;
  friend class lib::api::LevelTest;
  friend class SpriteTest;
  friend class objects::StaticObjectTest;
  SpriteFactory();
  // For testing only.
  SpriteFactory(unsigned int id, int texture_width, int texture_height);

  const bool make_mock_sprites_;
  const unsigned int id_testing_;
  const int texture_width_testing_;
  const int texture_height_testing_;
  absl::flat_hash_map<std::string, std::unique_ptr<Sprite>> sprites_;
};

}  // namespace sprites
}  // namespace api
}  // namespace lib

#endif  // LIB_API_SPRITES_SPRITE_FACTORY_H
