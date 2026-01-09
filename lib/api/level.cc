#include "raylib/include/raylib.h"

#include "lib/api/level.h"

#include <algorithm>
#include <optional>

#include "absl/log/check.h"
#include "lib/api/abilities/ability.h"
#include "lib/api/common_types.h"
#include "lib/api/controls.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace lib {
namespace api {

using abilities::Ability;
using api::ObjectAndAbilities;
using objects::MovableObject;
using objects::Object;
using objects::StaticObject;

namespace {

StaticObject MakeSpriteBoundingBox(const Object& object) {
  const float half_sprite_width =
      static_cast<float>(object.active_sprite_instance()->SpriteWidth()) / 2.0f;
  const float half_sprite_height =
      static_cast<float>(object.active_sprite_instance()->SpriteHeight()) /
      2.0f;
  const float upper_left_x = object.center().x - half_sprite_width;
  const float upper_left_y = object.center().y - half_sprite_height;

  return StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakeSpriteBoundingBox(),
      StaticObject::StaticObjectOpts{.is_hit_box_active = true,
                                     .should_draw_hit_box = false},
      FRectangle{.top_left = {upper_left_x, upper_left_y},
                 .width = static_cast<float>(
                     object.active_sprite_instance()->SpriteWidth()),
                 .height = static_cast<float>(
                     object.active_sprite_instance()->SpriteHeight())});
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
                                         native_screen_width_,
                                         native_screen_height_);
  }
}

void Level::UpdateCoordinateAxes() const {
  for (auto& coordinate_object : coordinate_objects_) {
    coordinate_object->ReAdjustToScreen(camera_.GetWorldPosition({0.0, 0.0}),
                                        native_screen_width_,
                                        native_screen_height_);
  }
}

void Level::Draw() const {
  // Inefficient, no need to sort everything.
  // Will work for now - can be improved in the future to only sort movable
  // objects.
  // Y-sorting.
  std::vector<std::pair<int, Object*>> objects_by_y_base;
  for (const auto& object : objects_) {
    objects_by_y_base.emplace_back(object->YBase(), object.get());
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
      {.x = native_screen_width_ / 2.0f, .y = native_screen_height_ / 2.0f});
  for (const auto& background_layer : background_layers_) {
    background_layer->Draw(
        /*draw_destination=*/{.x = center_world_pos.x,
                              .y = center_world_pos.y});
  }
}

void Level::MaybeClick(const ViewPortContext& ctx) const {
  // If not clicked or clicked outside the screen set every object's clicked
  // state to false.
  std::optional<const WorldPosition> cursor_pos_world =
      GetMouseWorldPosition(camera_, ctx, *controls_);
  if (!controls_->IsPrimaryPressed() || !cursor_pos_world.has_value()) {
    for (auto& object : objects_) {
      object->set_clicked(false);
    }
    return;
  }

  const StaticObject mouse_pointer = StaticObject(
      /*type=*/objects::ObjectTypeFactory::MakeMousePointer(),
      /*options=*/
      {.is_hit_box_active = true, .should_draw_hit_box = false},
      cursor_pos_world->ToFPoint());
  for (auto& object : objects_) {
    if (object->CollidesWith(mouse_pointer)) {
      object->set_clicked(true);
    }
  }
}

LevelId Level::Run(Stats& stats) {
  LevelId changed_id = id_;

  // Game does not always run in the same resolution as its logic. For the most
  // part, game will assume that it is running in native resolution and draw
  // everything on a virtual `native_screen_width_` X `native_screen_height_`
  // canvas.
  const float screen_width = static_cast<float>(GetScreenWidth());
  const float screen_height = static_cast<float>(GetScreenHeight());
  const ViewPortContext view_port_ctx(
      screen_width, screen_height, native_screen_width_, native_screen_height_);
  // `target` is the virtual canvas.
  const RenderTexture2D target =
      LoadRenderTexture(static_cast<int>(native_screen_width_),
                        static_cast<int>(native_screen_height_));
  SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
  const Rectangle source = {0.0f, 0.0f,
                            static_cast<float>(target.texture.width),
                            -static_cast<float>(target.texture.height)};
  // The actual screen on which the frame is drawn. Note letterboxing is done if
  // the ratios do not match.
  const Rectangle dest = {
      (screen_width - (native_screen_width_ * view_port_ctx.scale())) * 0.5f,
      (screen_height - (native_screen_height_ * view_port_ctx.scale())) * 0.5f,
      native_screen_width_ * view_port_ctx.scale(),
      native_screen_height_ * view_port_ctx.scale()};

  // Loop while the level is unchanged.
  while (changed_id == id_) {
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    DrawFPS(0, 0);
    // Get rid of deleted objects.
    CleanUpOrDie();
    camera_.MaybeActivate();
    UpdateScreenEdges();
    UpdateCoordinateAxes();
    MaybeClick(view_port_ctx);

    auto object_it = objects_.begin();
    auto ability_it = abilities_.begin();
    std::list<ObjectAndAbilities> new_objects_and_abilities;
    while (object_it != objects_.end() && ability_it != abilities_.end()) {
      for (const auto& ability : *ability_it) {
        std::list<ObjectAndAbilities> new_objects_and_abilities_current =
            ability->Use({.camera = camera_, .view_port_ctx = view_port_ctx});
        new_objects_and_abilities.splice(new_objects_and_abilities.end(),
                                         new_objects_and_abilities_current);
      }

      object_it->get()->Update(objects_);
      ++object_it;
      ++ability_it;
    }

    DrawBackgrounds();
    Draw();

    // Add all accumulated objects which abilities have spawned.
    for (auto& [object, abilities] : new_objects_and_abilities) {
      objects_.push_back(std::move(object));
      abilities_.push_back(std::move(abilities));
    }

    camera_.MaybeDeactivate();
    EndTextureMode();
    BeginDrawing();
    ClearBackground(BLACK);
    // Draw virtual canvas on the actual screen.
    DrawTexturePro(target.texture, source, dest, /*origin=*/{0, 0},
                   /*rotation=*/0.0f, WHITE);
    EndDrawing();
    changed_id = MaybeChangeLevel();
  }
  UnloadRenderTexture(target);
  return changed_id;
}

}  // namespace api
}  // namespace lib
