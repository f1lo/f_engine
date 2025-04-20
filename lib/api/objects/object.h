#ifndef LIB_API_OBJECTS_OBJECT_H
#define LIB_API_OBJECTS_OBJECT_H

#include <list>
#include <memory>

#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {

class Object {
 public:
  struct Opts {
    bool is_hit_box_active;
    bool should_draw_hit_box;
  };

  enum class Kind {
    UNSET,
    PLAYER,
    ENEMY,
    ABILITY,
    SCREEN_LEFT,
    SCREEN_RIGHT,
    SCREEN_TOP,
    SCREEN_BOTTOM,
  };

  explicit Object(const Kind kind, const Opts options, internal::HitBox hit_box)
      : kind_(kind), options_(options), hit_box_(std::move(hit_box)) {}

  virtual ~Object() = default;

  virtual void Update(
      const std::list<std::unique_ptr<Object>>& other_objects) = 0;
  virtual void Draw() const = 0;

  Kind kind() const { return kind_; }
  bool deleted() const { return deleted_; }
  const Opts& options() const { return options_; }

 protected:
  virtual bool OnCollisionCallback(const Object& other_object) = 0;
  bool UpdateInternal(const std::list<std::unique_ptr<Object>>& other_objects);

  internal::HitBox& mutable_hit_box() { return hit_box_; }
  Opts& mutable_options() { return options_; }
  void set_deleted(bool deleted) { deleted_ = deleted; }
  const internal::HitBox& hit_box() const { return hit_box_; }

 private:
  bool CollidesWith(const Object& other) const;

  Kind kind_;
  Opts options_;
  internal::HitBox hit_box_;

  bool deleted_ = false;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_H
