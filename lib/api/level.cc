#include "raylib/include/raylib.h"

#include "level.h"

#include "absl/container/flat_hash_map.h"
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

void Level::Run() {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    CleanUpOrDie();
    auto object_it = objects_.begin();
    auto ability_it = abilities_.begin();
    while (object_it != objects_.end() && ability_it != abilities_.end()) {
      for (const auto& ability : *ability_it) {
        ability->MaybeUseModifyUser(*object_it->get());
      }

      object_it->get()->Update(objects());
      object_it->get()->Draw();

      ++object_it;
      ++ability_it;
    }

    EndDrawing();
  }
}

void Level::AddScreenObjects() {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  std::unique_ptr<StaticObject> screen_left = std::make_unique<StaticObject>(
      SCREEN_LEFT,
      StaticObject::StaticObjectOpts(/*is_hit_box_active=*/true,
                                     /*should_draw_hit_box=*/false),
      std::vector({std::make_pair<double, double>(0, 0),
                   std::make_pair<double, double>(0, screen_height)}));
  std::unique_ptr<StaticObject> screen_right = std::make_unique<StaticObject>(
      SCREEN_RIGHT,
      StaticObject::StaticObjectOpts(/*is_hit_box_active=*/true,
                                     /*should_draw_hit_box=*/false),
      std::vector(
          {std::make_pair<double, double>(screen_width, 0),
           std::make_pair<double, double>(screen_width, screen_height)}));
  std::unique_ptr<StaticObject> screen_top = std::make_unique<StaticObject>(
      SCREEN_TOP,
      StaticObject::StaticObjectOpts(/*is_hit_box_active=*/true,
                                     /*should_draw_hit_box=*/false),
      std::vector({std::make_pair<double, double>(0, 0),
                   std::make_pair<double, double>(screen_width, 0)}));
  std::unique_ptr<StaticObject> screen_bottom = std::make_unique<StaticObject>(
      SCREEN_BOTTOM,
      StaticObject::StaticObjectOpts(/*is_hit_box_active=*/true,
                                     /*should_draw_hit_box=*/false),
      std::vector(
          {std::make_pair<double, double>(0, screen_height),
           std::make_pair<double, double>(screen_width, screen_height)}));

  add_object(std::move(screen_left));
  add_object(std::move(screen_right));
  add_object(std::move(screen_top));
  add_object(std::move(screen_bottom));
}

}  // namespace api
}  // namespace lib
