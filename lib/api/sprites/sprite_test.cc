#include <memory>
#include <string>

#include "absl/time/clock.h"
#include "absl/time/time.h"
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

constexpr unsigned int kTextureId = 7;
constexpr int kTextureWidth = 300;
constexpr int kTextureHeight = 200;
constexpr absl::Duration kAdvanceToNextFrameAfter = absl::Milliseconds(200);
constexpr absl::Duration kSmallIncrement = absl::Milliseconds(20);

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
  const std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeStaticSprite(resource_path);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};

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
  EXPECT_EQ(graphics->rotation(), 0.0f);
  EXPECT_EQ(sprite->SpriteHeight(), kTextureHeight);
}

TEST_F(SpriteTest, StaticSpriteRotateAndDraw) {
  const std::string resource_path = "a/b/picture.png";
  const std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeStaticSprite(resource_path);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};

  constexpr int degree = 90;
  sprite->RotateAndDraw(draw_destination, degree);

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
  EXPECT_EQ(graphics->rotation(), static_cast<float>(degree));
  EXPECT_EQ(sprite->SpriteHeight(), kTextureHeight);
}

TEST_F(SpriteTest, AnimatedSpriteNoFrameChange) {
  const std::string resource_path = "a/b/picture.png";
  constexpr int frame_count = 4;
  const std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeAnimatedSprite(resource_path, frame_count,
                                         kAdvanceToNextFrameAfter);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};

  absl::SleepFor(kAdvanceToNextFrameAfter / 2);
  sprite->Draw(draw_destination);

  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
  EXPECT_EQ(graphics->drawn_texture_source().x, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().y, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().width,
            static_cast<float>(kTextureWidth) / frame_count);
  EXPECT_EQ(graphics->drawn_texture_source().height,
            static_cast<float>(kTextureHeight));
  EXPECT_EQ(graphics->drawn_texture_origin().x,
            static_cast<float>(kTextureWidth) / frame_count / 2.0);
  EXPECT_EQ(graphics->drawn_texture_origin().y,
            static_cast<float>(kTextureHeight) / 2.0);
  EXPECT_EQ(graphics->drawn_texture().id, kTextureId);
  EXPECT_EQ(graphics->rotation(), 0.0f);
  EXPECT_EQ(sprite->SpriteHeight(), kTextureHeight);
}

TEST_F(SpriteTest, AnimatedSpriteFrameChange) {
  const std::string resource_path = "a/b/picture.png";
  constexpr int frame_count = 4;
  const std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeAnimatedSprite(resource_path, frame_count,
                                         kAdvanceToNextFrameAfter);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};

  sprite->Draw(draw_destination);
  absl::SleepFor(kAdvanceToNextFrameAfter + kSmallIncrement);
  sprite->Draw(draw_destination);

  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
  EXPECT_EQ(graphics->drawn_texture_source().x,
            static_cast<float>(kTextureWidth) / frame_count);
  EXPECT_EQ(graphics->drawn_texture_source().y, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().width,
            static_cast<float>(kTextureWidth) / frame_count);
  EXPECT_EQ(graphics->drawn_texture_source().height,
            static_cast<float>(kTextureHeight));
  EXPECT_EQ(graphics->drawn_texture_origin().x,
            static_cast<float>(kTextureWidth) / frame_count / 2.0);
  EXPECT_EQ(graphics->drawn_texture_origin().y,
            static_cast<float>(kTextureHeight) / 2.0);
  EXPECT_EQ(graphics->drawn_texture().id, kTextureId);
  EXPECT_EQ(graphics->rotation(), 0.0f);
  EXPECT_EQ(sprite->SpriteHeight(), kTextureHeight);
}

TEST_F(SpriteTest, AnimatedSpriteReset) {
  const std::string resource_path = "a/b/picture.png";
  constexpr int frame_count = 4;
  const std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeAnimatedSprite(resource_path, frame_count,
                                         kAdvanceToNextFrameAfter);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};
  sprite->Draw(draw_destination);
  absl::SleepFor(kAdvanceToNextFrameAfter + kSmallIncrement);
  sprite->Draw(draw_destination);

  sprite->Reset();
  sprite->Draw(draw_destination);

  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
  EXPECT_EQ(graphics->drawn_texture_source().x, 0);
  EXPECT_EQ(graphics->drawn_texture_source().y, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().width,
            static_cast<float>(kTextureWidth) / frame_count);
  EXPECT_EQ(graphics->drawn_texture_source().height,
            static_cast<float>(kTextureHeight));
  EXPECT_EQ(graphics->drawn_texture_origin().x,
            static_cast<float>(kTextureWidth) / frame_count / 2.0);
  EXPECT_EQ(graphics->drawn_texture_origin().y,
            static_cast<float>(kTextureHeight) / 2.0);
  EXPECT_EQ(graphics->drawn_texture().id, kTextureId);
  EXPECT_EQ(graphics->rotation(), 0.0f);
  EXPECT_EQ(sprite->SpriteHeight(), kTextureHeight);
}

TEST_F(SpriteTest, AnimatedSpriteLoopsBackToStart) {
  const std::string resource_path = "a/b/picture.png";
  constexpr int frame_count = 4;
  std::unique_ptr<SpriteInstance> sprite = sprite_factory_.MakeAnimatedSprite(
      resource_path, frame_count, kAdvanceToNextFrameAfter);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};

  sprite->Draw(draw_destination);
  absl::SleepFor(kAdvanceToNextFrameAfter + kSmallIncrement);
  sprite->Draw(draw_destination);
  absl::SleepFor(kAdvanceToNextFrameAfter + kSmallIncrement);
  sprite->Draw(draw_destination);
  absl::SleepFor(kAdvanceToNextFrameAfter + kSmallIncrement);
  sprite->Draw(draw_destination);
  absl::SleepFor(kAdvanceToNextFrameAfter + kSmallIncrement);
  sprite->Draw(draw_destination);

  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
  EXPECT_EQ(graphics->drawn_texture_source().x, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().y, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().width,
            static_cast<float>(kTextureWidth) / frame_count);
  EXPECT_EQ(graphics->drawn_texture_source().height,
            static_cast<float>(kTextureHeight));
  EXPECT_EQ(graphics->drawn_texture_origin().x,
            static_cast<float>(kTextureWidth) / frame_count / 2.0);
  EXPECT_EQ(graphics->drawn_texture_origin().y,
            static_cast<float>(kTextureHeight) / 2.0);
  EXPECT_EQ(graphics->drawn_texture().id, kTextureId);
  EXPECT_EQ(graphics->rotation(), 0.0f);
  EXPECT_EQ(sprite->SpriteHeight(), kTextureHeight);
}

TEST_F(SpriteTest, AnimatedSpriteRotateAndDraw) {
  const std::string resource_path = "a/b/picture.png";
  constexpr int frame_count = 4;
  const std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeAnimatedSprite(resource_path, frame_count,
                                         kAdvanceToNextFrameAfter);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};

  absl::SleepFor(kAdvanceToNextFrameAfter / 2);
  constexpr int degree = 90;
  sprite->RotateAndDraw(draw_destination, degree);

  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
  EXPECT_EQ(graphics->drawn_texture_source().x, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().y, 0.0f);
  EXPECT_EQ(graphics->drawn_texture_source().width,
            static_cast<float>(kTextureWidth) / frame_count);
  EXPECT_EQ(graphics->drawn_texture_source().height,
            static_cast<float>(kTextureHeight));
  EXPECT_EQ(graphics->drawn_texture_origin().x,
            static_cast<float>(kTextureWidth) / frame_count / 2.0);
  EXPECT_EQ(graphics->drawn_texture_origin().y,
            static_cast<float>(kTextureHeight) / 2.0);
  EXPECT_EQ(graphics->drawn_texture().id, kTextureId);
  EXPECT_EQ(graphics->rotation(), degree);
  EXPECT_EQ(sprite->SpriteHeight(), kTextureHeight);
}

TEST_F(SpriteTest, SpriteFactoryStaticSpriteAlreadyExists) {
  const std::string resource_path = "a/b/picture.png";
  std::unique_ptr<SpriteInstance> sprite =
      sprite_factory_.MakeStaticSprite(resource_path);

  sprite = sprite_factory_.MakeStaticSprite(resource_path);

  constexpr WorldPosition draw_destination{.x = 100, .y = 200};
  sprite->Draw(draw_destination);
  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
}

TEST_F(SpriteTest, SpriteFactoryAnimatedSpriteAlreadyExists) {
  const std::string resource_path = "a/b/picture.png";
  constexpr int frame_count = 4;
  std::unique_ptr<SpriteInstance> sprite = sprite_factory_.MakeAnimatedSprite(
      resource_path, frame_count, kAdvanceToNextFrameAfter);

  sprite = sprite_factory_.MakeAnimatedSprite(resource_path, frame_count,
                                              kAdvanceToNextFrameAfter);

  absl::SleepFor(kAdvanceToNextFrameAfter / 2);
  constexpr WorldPosition draw_destination{.x = 100, .y = 200};
  sprite->Draw(draw_destination);
  const GraphicsMock* graphics =
      dynamic_cast<const GraphicsMock*>(sprite->GraphicsForTesting());
  ASSERT_NE(graphics, nullptr);
  EXPECT_EQ(graphics->loaded_texture(), resource_path);
}

}  // namespace
}  // namespace sprites
}  // namespace api
}  // namespace lib
