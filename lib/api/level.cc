#include "raylib/include/raylib.h"

#include "lib/api/level.h"

#include <algorithm>
#include <optional>

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

using abilities::Ability;
using abilities::kKeyEscape;
using api::ObjectAndAbilities;
using lib::api::objects::kSpriteBoundingBox;
using objects::MovableObject;
using objects::Object;
using objects::StaticObject;

namespace {

StaticObject MakeSpriteBoundingBox(const Object& object) {
  const double half_sprite_width =
      static_cast<double>(object.active_sprite_instance()->SpriteWidth()) / 2;
  const double half_sprite_height =
      static_cast<double>(object.active_sprite_instance()->SpriteHeight()) / 2;
  const double upper_left_x = object.center().x - half_sprite_width;
  const double upper_left_y = object.center().y - half_sprite_height;
  const double lower_right_x = object.center().x + half_sprite_width;
  const double lower_right_y = object.center().y + half_sprite_height;

  return StaticObject(
      /*kind=*/kSpriteBoundingBox,
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>({{upper_left_x, upper_left_y},
                                              {upper_left_x, lower_right_y},
                                              {lower_right_x, upper_left_y},
                                              {lower_right_x, lower_right_y}}));
}

}  // namespace

LevelId Level::MaybeChangeLevel() const {
  if (controls_->IsPressed(kKeyEscape)) {
    return kExitLevel;
  }

  return id();
}

bool Level::ShouldDraw(const Object& object) const {
  if (screen_edge_objects_.empty()) {
    return true;
  }
  if (object.active_sprite_instance()) {
    const StaticObject sprite_bounding_box = MakeSpriteBoundingBox(object);
    for (const auto& screen_edge_object : screen_edge_objects_) {
      if (screen_edge_object->CollidesWith(sprite_bounding_box)) {
        return true;
      }
    }
  } else {
    for (const auto& screen_edge_object : screen_edge_objects_) {
      if (screen_edge_object->CollidesWith(object)) {
        return true;
      }
    }
  }
  return screen_edge_objects_[0]->center().x <= object.center().x &&
         object.center().x <= screen_edge_objects_[1]->center().x &&
         screen_edge_objects_[2]->center().y <= object.center().y &&
         object.center().y <= screen_edge_objects_[3]->center().y;
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
    coordinate_object->ReAdjustToScreen(camera_.GetWorldPosition({0.0, 0.0}),
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
  std::ranges::sort(objects_by_y_base);
  for (const auto& y_and_object : objects_by_y_base) {
    if (!ShouldDraw(*y_and_object.second)) {
      continue;
    }
    y_and_object.second->Draw();
  }
}

void Level::DrawBackgrounds() const {
  const WorldPosition center_world_pos = camera_.GetWorldPosition(
      {.x = GetScreenWidth() / 2.0, .y = GetScreenHeight() / 2.0});
  for (const auto& background_layer : background_layers_) {
    background_layer->Draw(
        /*draw_destination=*/{.x = center_world_pos.x,
                              .y = center_world_pos.y});
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

    DrawBackgrounds();
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
