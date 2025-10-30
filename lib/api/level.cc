#include "raylib/include/raylib.h"

#include "lib/api/level.h"

#include <optional>

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/objects/movable_object.h"

namespace lib {
namespace api {

using abilities::Ability;
using api::ObjectAndAbilities;
using lib::api::abilities::IsPressed;
using lib::api::abilities::kKeyEscape;
using objects::MovableObject;
using objects::Object;

LevelId Level::MaybeChangeLevel() const {
  if (IsPressed(kKeyEscape)) {
    return kExitLevel;
  }

  return id();
}

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

void Level::UpdateScreenEdges() const {
  for (auto& screen_edge_object : screen_edge_objects_) {
    screen_edge_object->ReAdjustToScreen(camera_.GetWorldPosition({0.0, 0.0}),
                                         GetScreenWidth(), GetScreenHeight());
  }
}

void Level::UpdateCoordinateAxes() const {
  for (auto& coordinate_object : coordinate_objects_) {
    coordinate_object->ReAdjustToScreen(
        camera_.GetWorldPosition({objects::kAxisOffset, objects::kAxisOffset}),
        GetScreenWidth(), GetScreenHeight());
  }
}

void Level::Draw() const {
  // Inefficient, no need to sort everything.
  // Will work for now - can be improved in the future to only sort movable
  // objects.
  // Y-sorting.
  std::vector<std::pair<int, Object*>> objects_by_y_base;
  for (const auto& object : objects_) {
    objects_by_y_base.push_back(std::make_pair(object->YBase(), object.get()));
  }
  sort(objects_by_y_base.begin(), objects_by_y_base.end());
  for (const auto& y_and_object : objects_by_y_base) {
    y_and_object.second->Draw();
  }
}

LevelId Level::Run() {
  LevelId changed_id = id_;
  while (changed_id == id_) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    CleanUpOrDie();
    camera_.MaybeActivate();
    UpdateScreenEdges();
    UpdateCoordinateAxes();

    auto object_it = objects_.begin();
    auto ability_it = abilities_.begin();
    std::list<ObjectAndAbilities> new_objects_and_abilities;
    while (object_it != objects_.end() && ability_it != abilities_.end()) {
      for (const auto& ability : *ability_it) {
        std::list<ObjectAndAbilities> new_objects_and_abilities_current =
            ability->Use(camera_);
        new_objects_and_abilities.splice(new_objects_and_abilities.end(),
                                         new_objects_and_abilities_current);
      }

      object_it->get()->Update(objects_);
      ++object_it;
      ++ability_it;
    }

    Draw();

    // Add all accumulated objects from abilities.
    for (auto& [object, abilities] : new_objects_and_abilities) {
      objects_.push_back(std::move(object));
      abilities_.push_back(std::move(abilities));
    }

    camera_.MaybeDeactivate();
    EndDrawing();
    changed_id = MaybeChangeLevel();
  }
  return changed_id;
}

}  // namespace api
}  // namespace lib
