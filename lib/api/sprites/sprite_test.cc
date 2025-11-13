#include <memory>
#include <string>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/common_types.h"
#include "lib/api/graphics_mock.h"
#include "lib/api/sprites/animated_sprite.h"
#include "lib/api/sprites/sprite.h"
#include "lib/api/sprites/sprite_factory.h"
#include "lib/api/sprites/sprite_instance.h"
#include "lib/api/sprites/static_sprite.h"

namespace lib {
namespace api {
namespace sprites {
namespace {

static constexpr unsigned int kTextureId = 7;
static constexpr int kTextureWidth = 300;
static constexpr int kTextureHeight = 200;

}  // namespace

class SpriteTest : public ::testing::Test {
 public:
  SpriteTest()
      : sprite_factory_(
            SpriteFactory(kTextureId, kTextureWidth, kTextureHeight)) {}

 protected:
  SpriteFactory sprite_factory_;
};

namespace {

TEST_F(SpriteTest, StaticSpriteDraw) {
  const std::string resource_path = "a/b/picture.png";
  std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeStaticSprite(resource_path);
  const WorldPosition draw_destination{.x = 100, .y = 200};

  sprite->Draw(draw_destination);

  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
  EXPECT_EQ(graphics->drawn_texture_source().x, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().y, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().width,
            static_cast<float>(kTextureWidth));
  EXPECT_EQ(graphics->drawn_texture_source().height,
            static_cast<float>(kTextureHeight));
  EXPECT_EQ(graphics->drawn_texture_origin().x,
            static_cast<float>(kTextureWidth) / 2.0);
  EXPECT_EQ(graphics->drawn_texture_origin().y,
            static_cast<float>(kTextureHeight) / 2.0);
  EXPECT_EQ(graphics->drawn_texture().id, kTextureId);
}

}  // namespace
}  // namespace sprites
}  // namespace api
}  // namespace lib
