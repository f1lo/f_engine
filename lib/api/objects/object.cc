#include "lib/api/objects/object.h"

#include <list>
#include <optional>

#include "lib/api/sprites/sprite_instance.h"

namespace lib {
namespace api {
namespace objects {

using sprites::SpriteInstance;

Object::Object(const Kind kind, const Opts& options, internal::HitBox hit_box,
               std::optional<std::unique_ptr<SpriteInstance>> sprite_instance)
    : kind_(kind),
      hit_box_(std::move(hit_box)),
      deleted_(false),
      clicked_(false),
      is_hit_box_active_(options.is_hit_box_active),
      should_draw_hit_box_(options.should_draw_hit_box) {
  if (sprite_instance.has_value()) {
    sprite_instance_ = std::move(*sprite_instance);
  } else {
    sprite_instance_ = nullptr;
  }
}

void Object::Draw() const {
  if (sprite_instance_) {
    sprite_instance_->Draw(center());
  }

  if (should_draw_hit_box()) {
    hit_box().Draw();
  }
}

bool Object::CollidesWith(const Object& other) const {
  // Hitbox not present or object deleted.
  if (deleted() || !is_hit_box_active()) {
    return false;
  }
  if (other.deleted() || !other.is_hit_box_active()) {
    return false;
  }

  return hit_box_.CollidesWith(other.hit_box_);
}

std::pair<double, double> Object::Reflect(const Object& other, double x,
                                          double y) const {
  // Hitbox not present or object deleted.
  if (deleted() || !is_hit_box_active()) {
    return {x, y};
  }

  return hit_box_.Reflect(other.hit_box_, x, y);
}

bool Object::UpdateInternal(
    const std::list<std::unique_ptr<Object>>& other_objects) {
  for (const auto& other_object : other_objects) {
    // Skip for the same object.
    if (this == other_object.get()) {
      continue;
    }
    // No collision - skip.
    if (!this->CollidesWith(*other_object.get())) {
      continue;
    }
    // Exit after first collision which changes the object state.
    if (this->OnCollisionCallback(*other_object.get())) {
      return true;
    }
  }

  return false;
}

int Object::YBase() const {
  if (!sprite_instance_) {
    // Technically incorrect - does not matter as long as there is no
    // sprite to draw.
    return (int)center().y;
  }

  return (int)center().y + sprite_instance_->MainSpriteHeight() / 2;
}

}  // namespace objects
}  // namespace api
}  // namespace lib