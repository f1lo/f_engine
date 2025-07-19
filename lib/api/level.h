#ifndef LIB_API_LEVEL_H
#define LIB_API_LEVEL_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>
#include <optional>

#include "abilities/ability.h"
#include "absl/container/flat_hash_map.h"
#include "lib/api/camera.h"
#include "lib/api/objects/object.h"
#include "lib/api/objects/static_object.h"

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
    level_ = std::make_unique<LevelT>(id);
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

  LevelBuilder& WithScreenObjects() {
    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    std::unique_ptr<objects::StaticObject> screen_left =
        std::make_unique<objects::StaticObject>(
            kScreenLeft,
            objects::StaticObject::StaticObjectOpts(
                /*is_hit_box_active=*/true,
                /*should_draw_hit_box=*/false),
            std::vector({std::make_pair<double, double>(0, 0),
                         std::make_pair<double, double>(0, screen_height)}));
    std::unique_ptr<objects::StaticObject> screen_right =
        std::make_unique<objects::StaticObject>(
            kScreenRight,
            objects::StaticObject::StaticObjectOpts(
                /*is_hit_box_active=*/true,
                /*should_draw_hit_box=*/false),
            std::vector(
                {std::make_pair<double, double>(screen_width, 0),
                 std::make_pair<double, double>(screen_width, screen_height)}));
    std::unique_ptr<objects::StaticObject> screen_top =
        std::make_unique<objects::StaticObject>(
            kScreenTop,
            objects::StaticObject::StaticObjectOpts(
                /*is_hit_box_active=*/true,
                /*should_draw_hit_box=*/false),
            std::vector({std::make_pair<double, double>(0, 0),
                         std::make_pair<double, double>(screen_width, 0)}));
    std::unique_ptr<objects::StaticObject> screen_bottom =
        std::make_unique<objects::StaticObject>(
            kScreenBottom,
            objects::StaticObject::StaticObjectOpts(
                /*is_hit_box_active=*/true,
                /*should_draw_hit_box=*/false),
            std::vector(
                {std::make_pair<double, double>(0, screen_height),
                 std::make_pair<double, double>(screen_width, screen_height)}));

    return AddObject(std::move(screen_left))
        .AddObject(std::move(screen_right))
        .AddObject(std::move(screen_top))
        .AddObject(std::move(screen_bottom));
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
  friend LevelBuilder;

  void CleanUpOrDie();
  [[nodiscard]] virtual LevelId MaybeChangeLevel() const = 0;
  LevelId id_;

 protected:
  explicit Level(const LevelId id) : id_(id) {}
  // This separation is required so that cyclic dependency is not introduced
  // between Object and Ability classes.
  std::list<std::unique_ptr<objects::Object>> objects_;
  std::list<std::list<std::unique_ptr<abilities::Ability>>> abilities_;
  Camera camera_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_LEVEL_H
