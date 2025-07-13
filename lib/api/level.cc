#include "raylib/include/raylib.h"

#include "lib/api/level.h"

#include <optional>

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

using objects::MovableObject;
using objects::Object;
using objects::StaticObject;

void Level::CleanUpOrDie() {
  auto object_it = objects_.begin();
  auto ability_it = abilities_.begin();
  CHECK(objects_.size() == abilities_.size())
      << "Abilities and Objects are out of sync, abilities size: "
      << abilities_.size() << ", objects size: " << objects_.size();
  while (object_it != objects_.end() && ability_it != abilities_.end()) {
    if (object_it->get()->deleted()) {
      object_it = objects_.erase(object_it);
      ability_it = abilities_.erase(ability_it);
    } else {
      ++object_it;
      ++ability_it;
    }
  }
}

LevelId Level::Run() {
  LevelId changed_id = id_;
  while (changed_id == id_) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    CleanUpOrDie();

    if (camera_object_ != nullptr) {
      BeginMode2D(Camera());
    }

    auto object_it = objects_.begin();
    auto ability_it = abilities_.begin();
    while (object_it != objects_.end() && ability_it != abilities_.end()) {
      for (const auto& ability : *ability_it) {
        ability->MaybeUseModifyUser();
      }

      object_it->get()->Update(objects_);
      object_it->get()->Draw();

      ++object_it;
      ++ability_it;
    }

    if (camera_object_ != nullptr) {
      EndMode2D();
    }
    EndDrawing();
    changed_id = MaybeChangeLevel();
  }
  return changed_id;
}

Camera2D& Level::Camera() {
  camera_.target.x = static_cast<float>(camera_object_->center_x());
  camera_.target.y = static_cast<float>(camera_object_->center_y());

  return camera_;
}

}  // namespace api
}  // namespace lib
