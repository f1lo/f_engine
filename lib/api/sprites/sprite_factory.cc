#include "lib/api/sprites/sprite_factory.h"

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "absl/log/check.h"
#include "absl/time/time.h"
#include "lib/api/graphics.h"
#include "lib/api/graphics_mock.h"
#include "lib/api/sprites/animated_sprite.h"
#include "lib/api/sprites/background_static_sprite.h"
#include "lib/api/sprites/sprite_instance.h"
#include "lib/api/sprites/static_sprite.h"

namespace lib {
namespace api {
namespace sprites {

SpriteFactory::SpriteFactory()
    : make_mock_sprites_(false),
      id_testing_(0),
      texture_width_testing_(0),
      texture_height_testing_(0) {}

SpriteFactory::SpriteFactory(const unsigned int id, const int texture_width,
                             const int texture_height)
    : make_mock_sprites_(true),
      id_testing_(id),
      texture_width_testing_(texture_width),
      texture_height_testing_(texture_height) {}

std::unique_ptr<SpriteInstance> SpriteFactory::MakeStaticSprite(
    const std::string_view resource_path) {
  auto [sprite_it, inserted] = sprites_.try_emplace(resource_path, nullptr);
  if (inserted) {
    if (make_mock_sprites_) {
      sprite_it->second = absl::WrapUnique(new StaticSprite(
          std::make_unique<GraphicsMock>(id_testing_, texture_width_testing_,
                                         texture_height_testing_),
          std::string(resource_path)));
    } else {
      sprite_it->second = absl::WrapUnique(new StaticSprite(
          std::make_unique<Graphics>(), std::string(resource_path)));
    }
  }

  std::unique_ptr<SpriteInstance> sprite_instance =
      absl::WrapUnique(new SpriteInstance(sprite_it->second.get()));

  return sprite_instance;
}

std::unique_ptr<SpriteInstance> SpriteFactory::MakeBackgroundStaticSprite(
    const std::string_view resource_path, const float parallax_factor) {
  CHECK(parallax_factor <= 1)
      << "parallax_factor > 1, should be between [0,1].";
  CHECK(parallax_factor >= 0)
      << "parallax_factor < 0, should be between [0,1].";
  auto [sprite_it, inserted] = sprites_.try_emplace(resource_path, nullptr);
  if (inserted) {
    if (make_mock_sprites_) {
      sprite_it->second = absl::WrapUnique(new BackgroundStaticSprite(
          std::make_unique<GraphicsMock>(id_testing_, texture_width_testing_,
                                         texture_height_testing_),
          std::string(resource_path), parallax_factor));
    } else {
      sprite_it->second = absl::WrapUnique(new BackgroundStaticSprite(
          std::make_unique<Graphics>(), std::string(resource_path),
          parallax_factor));
    }
  }

  std::unique_ptr<SpriteInstance> sprite_instance =
      absl::WrapUnique(new SpriteInstance(sprite_it->second.get()));

  return sprite_instance;
}

std::unique_ptr<SpriteInstance> SpriteFactory::MakeAnimatedSprite(
    const std::string_view resource_path, const int frame_count,
    const absl::Duration advance_to_next_frame_after) {
  auto [sprite_it, inserted] = sprites_.try_emplace(resource_path, nullptr);
  if (inserted) {
    if (make_mock_sprites_) {
      sprite_it->second = absl::WrapUnique(new AnimatedSprite(
          std::make_unique<GraphicsMock>(id_testing_, texture_width_testing_,
                                         texture_height_testing_),
          std::string(resource_path), frame_count));
    } else {
      new AnimatedSprite(std::make_unique<Graphics>(),
                         std::string(resource_path), frame_count);
    }
  }

  std::unique_ptr<SpriteInstance> sprite_instance = absl::WrapUnique(
      new SpriteInstance(sprite_it->second.get(), advance_to_next_frame_after));

  return sprite_instance;
}

}  // namespace sprites
}  // namespace api
}  // namespace lib
