#ifndef LIB_API_OBJECTS_OBJECT_H
#define LIB_API_OBJECTS_OBJECT_H

#include <list>
#include <memory>

#include "absl/base/nullability.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/sprites/sprite_instance.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {

typedef std::variant<FPoint, FLine, FRectangle, FCircle> HitBoxVariant;

class Object {
 public:
  struct Opts {
    bool is_hit_box_active;
    bool should_draw_hit_box;
  };

  Object(ObjectType type, const Opts& options, const HitBoxVariant& hit_box,
         std::unique_ptr<sprites::SpriteInstance> sprite_instance = nullptr);

  virtual ~Object() = default;

  virtual void Update(
      const std::list<std::unique_ptr<Object>>& other_objects) = 0;
  virtual void Draw() const;

  [[nodiscard]] std::pair<float, float> Reflect(const Object& other, float x,
                                                float y) const;
  [[nodiscard]] bool CollidesWith(const Object& other) const;
  [[nodiscard]] int YBase() const;

  // Object center in the world.
  [[nodiscard]] WorldPosition center() const {
    return {.x = hit_box().center_x(), .y = hit_box().center_y()};
  }
  [[nodiscard]] ObjectType type() const { return type_; }
  [[nodiscard]] bool deleted() const { return deleted_; }
  [[nodiscard]] bool clicked() const { return clicked_; }

  void set_deleted(const bool deleted) { deleted_ = deleted; }
  void set_clicked(const bool clicked) { clicked_ = clicked; }

  [[nodiscard]] absl::Nullable<const sprites::SpriteInstance*>
  active_sprite_instance() const {
    return active_sprite_instance_.get();
  }

 protected:
  [[nodiscard]] bool is_hit_box_active() const { return is_hit_box_active_; }
  [[nodiscard]] bool should_draw_hit_box() const {
    return should_draw_hit_box_;
  }
  virtual bool OnCollisionCallback(Object& other_object) = 0;
  bool UpdateInternal(const std::list<std::unique_ptr<Object>>& other_objects);

  internal::HitBox& mutable_hit_box() { return hit_box_; }
  [[nodiscard]] const internal::HitBox& hit_box() const { return hit_box_; }

 private:
  ObjectType type_;
  bool deleted_;
  bool clicked_;
  const bool is_hit_box_active_;
  const bool should_draw_hit_box_;
  internal::HitBox hit_box_;
  std::unique_ptr<sprites::SpriteInstance> active_sprite_instance_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_H
