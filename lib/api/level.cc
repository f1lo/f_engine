#include "raylib/include/raylib.h"

#include "level.h"

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/objects/movable_object.h"

namespace lib {
namespace api {

using objects::MovableObject;
using objects::Object;

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

void Level::Run() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    CleanUpOrDie();
    auto object_it = objects_.begin();
    auto ability_it = abilities_.begin();
    while (object_it != objects_.end() && ability_it != abilities_.end()) {
      for (const auto& ability : *ability_it) {
        auto [update, spawned_object] = ability->MaybeUse();
        if (update.has_value()) {
          object_it->get()->ApplyPendingUpdate(*update);
        }
        if (spawned_object) {
          // TODO(f1lo): Implement.
        }
      }

      object_it->get()->Update(objects());
      object_it->get()->Draw();

      ++object_it;
      ++ability_it;
    }

    EndDrawing();
  }
}

}  // namespace api
}  // namespace lib
