#include "player_controllable_object.h"

#include <vector>

#include "absl/log/check.h"
#include "absl/status/statusor.h"
#include "lib/internal/geometry/shape.h"
#include "lib/internal/hit_box.h"

namespace lib {
namespace api {

using internal::HitBox;
using internal::Point;

namespace {
HitBox CreateHitBoxOrDie(const std::vector<std::pair<int, int>> &vertices) {
  // Convert pairs to Points.
  std::vector<Point> points;
  points.reserve(vertices.size());
  for (const auto &[x, y]: vertices) {
    points.emplace_back(x, y);
  }

  absl::StatusOr<HitBox> hit_box = HitBox::CreateHitBox(std::move(points));
  CHECK(hit_box.ok()) << "HitBox::CreateHitBox() failed " << hit_box.status();

  return *std::move(hit_box);
}

HitBox CreateCircle(const int x, const int y, const uint32_t radius) {
  return HitBox::CreateHitBox(Point{x, y}, radius);
}

} // namespace

PlayerControllableObject::PlayerControllableObject(
    const std::vector<std::pair<int, int>> &hit_box_vertices,
    const ControllableObjectCreationOpts opts) :
    is_hit_box_active_(opts.is_hit_box_active),
    should_draw_hit_box_(opts.should_draw_hit_box),
    hit_box_(CreateHitBoxOrDie(hit_box_vertices)) {}

PlayerControllableObject::PlayerControllableObject(
    const std::pair<int, int> hit_box_center, const uint32_t hit_box_radius,
    const ControllableObjectCreationOpts opts) :
    is_hit_box_active_(opts.is_hit_box_active),
    should_draw_hit_box_(opts.should_draw_hit_box),
    hit_box_(CreateCircle(hit_box_center.first, hit_box_center.second,
                          hit_box_radius)) {}

// Assumes `BeginDrawing` has been called.
void PlayerControllableObject::Draw() {
  // TODO(f1lo): Draw texture in the future.
  if (!should_draw_hit_box_) {
    return;
  }
  hit_box_.Draw();
}

void PlayerControllableObject::Move(int pos_x, int pos_y) {}

} // namespace api
} // namespace lib
