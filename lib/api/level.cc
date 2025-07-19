#include "raylib/include/raylib.h"

#include "lib/api/level.h"

#include <optional>

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

using abilities::Ability;
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
    camera_.MaybeActivate();

    auto object_it = objects_.begin();
    auto ability_it = abilities_.begin();
    std::list<std::unique_ptr<Object>> abilities_objects;
    while (object_it != objects_.end() && ability_it != abilities_.end()) {
      for (const auto& ability : *ability_it) {
        std::list<std::unique_ptr<Object>> new_objects = ability->Use(camera_);
        abilities_objects.splice(abilities_objects.end(), new_objects);
      }

      object_it->get()->Update(objects_);
      object_it->get()->Draw();

      ++object_it;
      ++ability_it;
    }

    // Add all accumulated objects from abilities.
    abilities_.splice(abilities_.end(),
                      std::list<std::list<std::unique_ptr<Ability>>>(
                          abilities_objects.size()));
    objects_.splice(objects_.end(), abilities_objects);

    camera_.MaybeDeactivate();
    EndDrawing();
    changed_id = MaybeChangeLevel();
  }
  return changed_id;
}

}  // namespace api
}  // namespace lib
