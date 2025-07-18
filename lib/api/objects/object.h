#ifndef LIB_API_OBJECTS_OBJECT_H
#define LIB_API_OBJECTS_OBJECT_H

#include <list>
#include <memory>

#include "lib/internal/hit_box.h"

typedef uint32_t Kind;

static constexpr Kind kPlayer = std::numeric_limits<uint32_t>::max() - 1;
static constexpr Kind kEnemy = std::numeric_limits<uint32_t>::max() - 2;
static constexpr Kind kScreenLeft = std::numeric_limits<uint32_t>::max() - 3;
static constexpr Kind kScreenRight = std::numeric_limits<uint32_t>::max() - 4;
static constexpr Kind kScreenTop = std::numeric_limits<uint32_t>::max() - 5;
static constexpr Kind kScreenBottom = std::numeric_limits<uint32_t>::max() - 6;
static constexpr Kind kMousePointer = std::numeric_limits<uint32_t>::max() - 7;
static constexpr Kind kButton = std::numeric_limits<uint32_t>::max() - 8;

namespace lib {
namespace api {
namespace objects {

class Object {
 public:
  struct Opts {
    bool is_hit_box_active;
    bool should_draw_hit_box;
  };

  explicit Object(const Kind kind, const Opts options, internal::HitBox hit_box)
      : kind_(kind),
        options_(options),
        hit_box_(std::move(hit_box)),
        deleted_(false),
        clicked_(false) {}

  virtual ~Object() = default;

  virtual void Update(
      const std::list<std::unique_ptr<Object>>& other_objects) = 0;
  virtual void Draw() const = 0;

  [[nodiscard]] std::pair<double, double> Reflect(const Object& other, double x,
                                                  double y) const;
  [[nodiscard]] bool CollidesWith(const Object& other) const;

  [[nodiscard]] double center_x() const { return hit_box_.center_x(); }
  [[nodiscard]] double center_y() const { return hit_box_.center_y(); }
  [[nodiscard]] Kind kind() const { return kind_; }
  [[nodiscard]] bool deleted() const { return deleted_; }
  [[nodiscard]] bool clicked() const { return clicked_; }

  void set_deleted(const bool deleted) { deleted_ = deleted; }
  void set_clicked(const bool clicked) { clicked_ = clicked; }

 protected:
  [[nodiscard]] const Opts& options() const { return options_; }
  virtual bool OnCollisionCallback(Object& other_object) = 0;
  bool UpdateInternal(const std::list<std::unique_ptr<Object>>& other_objects);

  internal::HitBox& mutable_hit_box() { return hit_box_; }
  [[nodiscard]] const internal::HitBox& hit_box() const { return hit_box_; }

 private:
  Kind kind_;
  Opts options_;
  internal::HitBox hit_box_;
  bool deleted_;
  bool clicked_;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_H
