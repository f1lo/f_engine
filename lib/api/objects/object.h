#ifndef LIB_API_OBJECTS_OBJECT_H
#define LIB_API_OBJECTS_OBJECT_H

#include <list>
#include <memory>

#include "absl/container/flat_hash_map.h"
#include "absl/functional/any_invocable.h"
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

  struct PendingUpdate {
    std::optional<bool> deleted;
    std::optional<bool> is_hit_box_active;

    std::optional<int> velocity_x;
    std::optional<int> velocity_y;
  };

  enum class Kind { UNSET, PLAYER, ENEMY, PROJECTILE };

  typedef absl::AnyInvocable<PendingUpdate(const Object& to_update,
                                           const Object& other) const>
      CollisionCallback;

  explicit Object(
      const Kind kind, const Opts options,
      absl::flat_hash_map<Kind, CollisionCallback> object_collision_callbacks,
      internal::HitBox hit_box)
      : kind_(kind),
        options_(options),
        object_collision_callbacks_(std::move(object_collision_callbacks)),
        hit_box_(std::move(hit_box)) {}

  virtual ~Object() = default;

  virtual void Update(
      const std::list<std::unique_ptr<Object>>& other_objects) = 0;
  virtual void Draw() const = 0;
  virtual void ApplyPendingUpdate(const PendingUpdate& other) = 0;

  Kind kind() const { return kind_; }
  bool deleted() const { return deleted_; }
  const internal::HitBox& hit_box() const { return hit_box_; }
  const Opts& options() const { return options_; }
  const absl::flat_hash_map<Kind, CollisionCallback>&
  object_collision_callbacks() const {
    return object_collision_callbacks_;
  }

 protected:
  internal::HitBox& mutable_hit_box() { return hit_box_; }
  Opts& mutable_options() { return options_; }
  void set_deleted(bool deleted) { deleted_ = deleted; }

 private:
  Kind kind_;
  Opts options_;
  absl::flat_hash_map<Kind, CollisionCallback> object_collision_callbacks_;
  internal::HitBox hit_box_;

  bool deleted_ = false;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_H
