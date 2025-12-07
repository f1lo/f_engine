#ifndef LIB_API_LEVEL_H
#define LIB_API_LEVEL_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/log/check.h"
#include "absl/memory/memory.h"
#include "gtest/gtest_prod.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/camera.h"
#include "lib/api/controls.h"
#include "lib/api/objects/coordinate_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/screen_edge_object.h"
#include "lib/api/objects/static_object.h"
#include "lib/api/sprites/sprite_instance.h"
#include "lib/api/stats.h"

namespace lib {
namespace api {

constexpr double kWorldBorderLength = 1000000;

typedef uint32_t LevelId;

static constexpr LevelId kInvalidLevel =
    std::numeric_limits<uint32_t>::max() - 1;
static constexpr LevelId kExitLevel = std::numeric_limits<uint32_t>::max() - 2;
static constexpr LevelId kTitleScreenLevel =
    std::numeric_limits<uint32_t>::max() - 3;
static constexpr LevelId kFirstLevel = std::numeric_limits<uint32_t>::max() - 4;

template <typename LevelT>
class LevelBuilder {
 public:
  virtual ~LevelBuilder() = default;
  explicit LevelBuilder(const LevelId id, const float native_screen_width,
                        const float native_screen_height)
      : native_screen_width_(native_screen_width),
        native_screen_height_(native_screen_height) {
    level_ = absl::WrapUnique(
        new LevelT(id, native_screen_width_, native_screen_height_));
  }

  LevelBuilder& AddObjectAndAbilities(
      std::unique_ptr<objects::Object> object,
      std::list<std::unique_ptr<abilities::Ability>> abilities,
      const bool attach_camera = false) {
    if (attach_camera) {
      level_->camera_.Bind(object.get());
    }

    for (const auto& ability : abilities) {
      ability->set_user(object.get());
    }

    level_->objects_.emplace_back(std::move(object));
    level_->abilities_.emplace_back(std::move(abilities));

    return *this;
  }

  LevelBuilder& AddObject(std::unique_ptr<objects::Object> object,
                          const bool attach_camera = false) {
    if (attach_camera) {
      level_->camera_.Bind(object.get());
    }

    level_->objects_.emplace_back(std::move(object));
    level_->abilities_.emplace_back();

    return *this;
  }

  LevelBuilder& AddBackgroundLayer(
      std::unique_ptr<sprites::SpriteInstance> layer) {
    level_->background_layers_.push_back({std::move(layer)});

    return *this;
  }

  LevelBuilder& WithScreenObjects(const bool should_draw_hitbox = false) {
    CHECK(level_->screen_edge_objects_.empty())
        << "WithScreenObjects() has already been called.";
    std::unique_ptr<objects::ScreenEdgeObject> screen_left =
        objects::ScreenEdgeObject::MakeLeft(native_screen_height_,
                                            should_draw_hitbox);
    std::unique_ptr<objects::ScreenEdgeObject> screen_right =
        objects::ScreenEdgeObject::MakeRight(
            native_screen_width_, native_screen_height_, should_draw_hitbox);
    std::unique_ptr<objects::ScreenEdgeObject> screen_top =
        objects::ScreenEdgeObject::MakeTop(native_screen_width_,
                                           should_draw_hitbox);
    std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
        objects::ScreenEdgeObject::MakeBottom(
            native_screen_width_, native_screen_height_, should_draw_hitbox);

    level_->screen_edge_objects_.emplace_back(screen_left.get());
    level_->screen_edge_objects_.emplace_back(screen_right.get());
    level_->screen_edge_objects_.emplace_back(screen_top.get());
    level_->screen_edge_objects_.emplace_back(screen_bottom.get());
    return AddObject(std::move(screen_left))
        .AddObject(std::move(screen_right))
        .AddObject(std::move(screen_top))
        .AddObject(std::move(screen_bottom));
  }

  LevelBuilder& WithWorldBorderX(const double x,
                                 const bool should_draw_hitbox = false) {
    std::unique_ptr<objects::StaticObject> x_border =
        std::make_unique<objects::StaticObject>(
            objects::ObjectTypeFactory::MakeWorldBorder(),
            objects::StaticObject::StaticObjectOpts{
                .is_hit_box_active = true,
                .should_draw_hit_box = should_draw_hitbox},
            std::vector<std::pair<double, double>>{{x, -kWorldBorderLength},
                                                   {x, kWorldBorderLength}});

    level_->world_border_objects_.emplace_back(x_border.get());
    return AddObject(std::move(x_border));
  }

  LevelBuilder& WithWorldBorderY(const double y,
                                 const bool should_draw_hitbox = false) {
    std::unique_ptr<objects::StaticObject> y_border =
        std::make_unique<objects::StaticObject>(
            objects::ObjectTypeFactory::MakeWorldBorder(),
            objects::StaticObject::StaticObjectOpts{
                .is_hit_box_active = true,
                .should_draw_hit_box = should_draw_hitbox},
            std::vector<std::pair<double, double>>{{-kWorldBorderLength, y},
                                                   {kWorldBorderLength, y}});

    level_->world_border_objects_.emplace_back(y_border.get());
    return AddObject(std::move(y_border));
  }

  LevelBuilder& WithCoordinates() {
    CHECK(level_->coordinate_objects_.empty())
        << "WithCoordinates() has already been called.";
    std::unique_ptr<objects::CoordinateObject> x_axis =
        objects::CoordinateObject::MakeX(native_screen_width_,
                                         native_screen_height_);
    std::unique_ptr<objects::CoordinateObject> y_axis =
        objects::CoordinateObject::MakeY(native_screen_width_,
                                         native_screen_height_);

    level_->coordinate_objects_.emplace_back(x_axis.get());
    level_->coordinate_objects_.emplace_back(y_axis.get());
    return AddObject(std::move(x_axis)).AddObject(std::move(y_axis));
  }

  virtual std::unique_ptr<LevelT> Build() { return std::move(level_); }

 protected:
  const float native_screen_width_;
  const float native_screen_height_;

  std::unique_ptr<LevelT> level_;
};

class Level {
 public:
  virtual ~Level() = default;

  LevelId Run(Stats& stats);
  [[nodiscard]] LevelId id() const { return id_; }

 private:
  template <typename LevelT>
  friend class LevelBuilder;

  void CleanUpOrDie();
  [[nodiscard]] virtual LevelId MaybeChangeLevel() const;
  void UpdateScreenEdges() const;
  void UpdateCoordinateAxes() const;
  [[nodiscard]] bool ShouldDraw(const objects::Object& object) const;
  void Draw() const;
  void DrawBackgrounds() const;
  LevelId id_;

 protected:
  Level(const LevelId id, const float native_screen_width,
        const float native_screen_height)
      : id_(id),
        camera_(native_screen_width, native_screen_height),
        controls_(std::make_unique<Controls>()),
        native_screen_width_(native_screen_width),
        native_screen_height_(native_screen_height) {}
  // This separation is required so that cyclic dependency is not introduced
  // between Object and Ability classes.
  FRIEND_TEST(LevelTest, ObjectsAreAdded);
  FRIEND_TEST(LevelTest, ObjectsAndAbilitiesAreAdded);
  FRIEND_TEST(LevelTest, ScreenEdgeObjects);
  FRIEND_TEST(LevelTest, CoordinateObjects);
  FRIEND_TEST(LevelTest, CleanupOrDie);
  FRIEND_TEST(LevelTest, WorldBorderObjects);
  FRIEND_TEST(LevelTest, DrawsNoScreenEdgeObjects);
  FRIEND_TEST(LevelTest, DoesNotDrawOutsideScreenOnlyHitBox);
  FRIEND_TEST(LevelTest, DrawsPartiallyOutsideScreenOnlyHitBox);
  FRIEND_TEST(LevelTest, DrawsFullyInsideScreenOnlyHitBox);
  FRIEND_TEST(LevelTest, DoesNotDrawOutsideScreen);
  FRIEND_TEST(LevelTest, DrawsPartiallyOutsideScreen);
  FRIEND_TEST(LevelTest, DrawsFullyInsideScreen);
  FRIEND_TEST(LevelTest, DrawBackground);
  FRIEND_TEST(LevelDeathTest, CleanUpOrDieOutOfSync);
  FRIEND_TEST(TitleScreenLevelTest, StartAndExitAddedOk);
  std::list<std::unique_ptr<objects::Object>> objects_;
  std::list<std::list<std::unique_ptr<abilities::Ability>>> abilities_;
  std::vector<objects::ScreenEdgeObject*> screen_edge_objects_;
  std::list<objects::CoordinateObject*> coordinate_objects_;
  std::list<objects::StaticObject*> world_border_objects_;
  Camera camera_;
  std::unique_ptr<const Controls> controls_;
  std::vector<std::unique_ptr<sprites::SpriteInstance>> background_layers_;

  const float native_screen_width_;
  const float native_screen_height_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_LEVEL_H
