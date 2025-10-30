#ifndef LIB_API_LEVEL_H
#define LIB_API_LEVEL_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>
#include <optional>

#include "abilities/ability.h"
#include "absl/container/flat_hash_map.h"
#include "absl/log/check.h"
#include "absl/memory/memory.h"
#include "gtest/gtest_prod.h"
#include "lib/api/camera.h"
#include "lib/api/objects/coordinate_object.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/screen_edge_object.h"

namespace lib {
namespace api {

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
  explicit LevelBuilder(const LevelId id) {
    level_ = absl::WrapUnique(new LevelT(id));
  }

  LevelBuilder& AddObjectAndAbilities(
      std::unique_ptr<objects::Object> object,
      std::list<std::unique_ptr<abilities::Ability>> abilities,
      const bool attach_camera = false) {
    if (attach_camera) {
      level_->camera_.Bind(object.get());
    }

    for (const auto& ability : abilities) {
      ability->user_ = object.get();
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

  LevelBuilder& WithScreenObjects(const bool should_draw_hitbox = false) {
    CHECK(level_->screen_edge_objects_.empty())
        << "WithScreenObjects() has already been called.";
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();
    std::unique_ptr<objects::ScreenEdgeObject> screen_left =
        objects::ScreenEdgeObject::MakeLeft(screen_height, should_draw_hitbox);
    std::unique_ptr<objects::ScreenEdgeObject> screen_right =
        objects::ScreenEdgeObject::MakeRight(screen_width, screen_height,
                                             should_draw_hitbox);
    std::unique_ptr<objects::ScreenEdgeObject> screen_top =
        objects::ScreenEdgeObject::MakeTop(screen_width, should_draw_hitbox);
    std::unique_ptr<objects::ScreenEdgeObject> screen_bottom =
        objects::ScreenEdgeObject::MakeBottom(screen_width, screen_height,
                                              should_draw_hitbox);

    level_->screen_edge_objects_.emplace_back(screen_left.get());
    level_->screen_edge_objects_.emplace_back(screen_right.get());
    level_->screen_edge_objects_.emplace_back(screen_top.get());
    level_->screen_edge_objects_.emplace_back(screen_bottom.get());
    return AddObject(std::move(screen_left))
        .AddObject(std::move(screen_right))
        .AddObject(std::move(screen_top))
        .AddObject(std::move(screen_bottom));
  }

  LevelBuilder& WithCoordinates() {
    CHECK(level_->coordinate_objects_.empty())
        << "WithCoordinates() has already been called.";
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();
    std::unique_ptr<objects::CoordinateObject> x_axis =
        objects::CoordinateObject::MakeX(screen_width);
    std::unique_ptr<objects::CoordinateObject> y_axis =
        objects::CoordinateObject::MakeY(screen_height);

    level_->coordinate_objects_.emplace_back(x_axis.get());
    level_->coordinate_objects_.emplace_back(y_axis.get());
    return AddObject(std::move(x_axis)).AddObject(std::move(y_axis));
  }

  virtual std::unique_ptr<LevelT> Build() { return std::move(level_); }

 protected:
  std::unique_ptr<LevelT> level_;
};

class Level {
 public:
  virtual ~Level() = default;

  LevelId Run();
  [[nodiscard]] LevelId id() const { return id_; }

 private:
  template <typename LevelT>
  friend class LevelBuilder;

  void CleanUpOrDie();
  [[nodiscard]] virtual LevelId MaybeChangeLevel() const;
  void UpdateScreenEdges() const;
  void UpdateCoordinateAxes() const;
  void Draw() const;
  LevelId id_;

 protected:
  explicit Level(const LevelId id) : id_(id) {}
  // This separation is required so that cyclic dependency is not introduced
  // between Object and Ability classes.
  FRIEND_TEST(LevelTest, ObjectsAreAdded);
  FRIEND_TEST(LevelTest, ObjectsAndAbilitiesAreAdded);
  FRIEND_TEST(LevelTest, ScreenEdgeObjectsAreAdded);
  FRIEND_TEST(LevelTest, CoordinateObjectsAreAdded);
  FRIEND_TEST(TitleScreenLevelTest, StartAndExitAddedOk);
  std::list<std::unique_ptr<objects::Object>> objects_;
  std::list<std::list<std::unique_ptr<abilities::Ability>>> abilities_;
  std::list<objects::ScreenEdgeObject*> screen_edge_objects_;
  std::list<objects::CoordinateObject*> coordinate_objects_;
  Camera camera_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_LEVEL_H
