#ifndef LIB_API_OBJECTS_OBJECT_H
#define LIB_API_OBJECTS_OBJECT_H

#include <list>
#include <memory>
#include <optional>

#include "lib/api/common_types.h"
#include "lib/api/sprites/sprite_instance.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {
namespace objects {

typedef uint32_t Kind;

static constexpr Kind kPlayer = std::numeric_limits<uint32_t>::max() - 1;
static constexpr Kind kEnemy = std::numeric_limits<uint32_t>::max() - 2;
static constexpr Kind kScreenLeft = std::numeric_limits<uint32_t>::max() - 3;
static constexpr Kind kScreenRight = std::numeric_limits<uint32_t>::max() - 4;
static constexpr Kind kScreenTop = std::numeric_limits<uint32_t>::max() - 5;
static constexpr Kind kScreenBottom = std::numeric_limits<uint32_t>::max() - 6;
static constexpr Kind kMousePointer = std::numeric_limits<uint32_t>::max() - 7;
static constexpr Kind kButton = std::numeric_limits<uint32_t>::max() - 8;
static constexpr Kind kProjectilePlayer =
    std::numeric_limits<uint32_t>::max() - 9;
static constexpr Kind kCoordinate = std::numeric_limits<uint32_t>::max() - 10;
static constexpr Kind kWorldBorder = std::numeric_limits<uint32_t>::max() - 11;

class Object {
 public:
  struct Opts {
    bool is_hit_box_active;
    bool should_draw_hit_box;
  };

  explicit Object(const Kind kind, const Opts& options,
                  internal::HitBox hit_box,
                  std::optional<std::unique_ptr<sprites::SpriteInstance>>
                      sprite_instance = std::nullopt);

  virtual ~Object() = default;

  virtual void Update(
      const std::list<std::unique_ptr<Object>>& other_objects) = 0;
  virtual void Draw() const;

  [[nodiscard]] std::pair<double, double> Reflect(const Object& other, double x,
                                                  double y) const;
  [[nodiscard]] bool CollidesWith(const Object& other) const;
  int YBase() const;

  // Object center in the world.
  [[nodiscard]] WorldPosition center() const {
    return {.x = hit_box().center_x(), .y = hit_box().center_y()};
  }
  [[nodiscard]] Kind kind() const { return kind_; }
  [[nodiscard]] bool deleted() const { return deleted_; }
  [[nodiscard]] bool clicked() const { return clicked_; }

  void set_deleted(const bool deleted) { deleted_ = deleted; }
  void set_clicked(const bool clicked) { clicked_ = clicked; }

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
  Kind kind_;
  internal::HitBox hit_box_;
  bool deleted_;
  bool clicked_;
  const bool is_hit_box_active_;
  const bool should_draw_hit_box_;
  std::unique_ptr<sprites::SpriteInstance> sprite_instance_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_H
