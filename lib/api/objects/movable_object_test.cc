#include "lib/api/objects/movable_object.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace objects {
namespace {

class DummyMovableObject : public MovableObject {
 public:
  DummyMovableObject(const double velocity,
                     const std::pair<double, double> hit_box_center)
      : MovableObject(
            /*kind=*/kPlayer,
            MovableObjectOpts(/*is_hit_box_active=*/false,
                              /*should_draw_hit_box=*/false,
                              /*attach_camera=*/false, velocity),
            hit_box_center, /*hit_box_radius=*/3) {}

  bool OnCollisionCallback(Object& other_object) override { return false; }
  void Move() { MovableObject::Move(); }
  void ResetLastMove() { MovableObject::ResetLastMove(); }
};

TEST(MovableObjectTest, SetDirectionGlobal) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));

  movable_object.SetDirectionGlobal(3, 4);

  EXPECT_DOUBLE_EQ(movable_object.direction_x(), 0.6);
  EXPECT_DOUBLE_EQ(movable_object.direction_y(), 0.8);
}

TEST(MovableObjectTest, SetDirectionRelative) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(1, 2));

  movable_object.SetDirectionRelative(1, 4);

  EXPECT_DOUBLE_EQ(movable_object.direction_x(), 0);
  EXPECT_DOUBLE_EQ(movable_object.direction_y(), 1);
}

TEST(MovableObjectTest, Move) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(1, 2));
  movable_object.SetDirectionRelative(1, 4);

  movable_object.Move();

  EXPECT_EQ(movable_object.center(), (WorldPosition{.x = 1, .y = 7}));
}

TEST(MovableObjectTest, ResetLastMove) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(1, 2));
  movable_object.SetDirectionRelative(1, 4);

  movable_object.Move();
  movable_object.ResetLastMove();

  EXPECT_EQ(movable_object.center(), (WorldPosition{.x = 1, .y = 2}));
}

TEST(MovableObjectTest, FrozenObjectDoesNotMove) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(1, 2));
  movable_object.SetDirectionRelative(1, 4);

  movable_object.freeze_until_next_set_direction();
  movable_object.Move();

  EXPECT_EQ(movable_object.center(), (WorldPosition{.x = 1, .y = 2}));
}

TEST(MovableObjectTest, SetDirectionGlobalUnfreezes) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(1, 2));

  movable_object.freeze_until_next_set_direction();
  movable_object.SetDirectionGlobal(0, -1);
  movable_object.Move();

  EXPECT_EQ(movable_object.center(), (WorldPosition{.x = 1, .y = -3}));
}

TEST(MovableObjectTest, SetDirectionRelativeUnfreezes) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(1, 2));

  movable_object.freeze_until_next_set_direction();
  movable_object.SetDirectionRelative(1, -1);
  movable_object.Move();

  EXPECT_EQ(movable_object.center(), (WorldPosition{.x = 1, .y = -3}));
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib
