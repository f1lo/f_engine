#include "lib/api/objects/movable_object.h"

#include <list>
#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/common_types.h"
#include "lib/api/objects/object_type.h"

namespace lib {
namespace api {
namespace objects {
namespace {

class DummyMovableObject : public MovableObject {
 public:
  DummyMovableObject(const float velocity,
                     const std::pair<float, float> hit_box_center)
      : MovableObject(
            /*type=*/ObjectTypeFactory::MakePlayer(),
            MovableObjectOpts{.is_hit_box_active = true,
                              .should_draw_hit_box = false,
                              .attach_camera = false,
                              .velocity = velocity},
            hit_box_center, /*hit_box_radius=*/3) {}

  DummyMovableObject(
      const float velocity,
      const std::vector<std::pair<float, float>>& hit_box_vertices)
      : MovableObject(
            /*type=*/ObjectTypeFactory::MakePlayer(),
            MovableObjectOpts{.is_hit_box_active = true,
                              .should_draw_hit_box = false,
                              .attach_camera = false,
                              .velocity = velocity},
            hit_box_vertices) {}

  bool OnCollisionCallback(Object& other_object) override {
    set_deleted(true);
    return true;
  }
  void Move() override { MovableObject::Move(); }
  void ResetLastMove() override { MovableObject::ResetLastMove(); }
};

TEST(MovableObjectTest, SetDirectionGlobal) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(0, 0));

  movable_object.SetDirectionGlobal(3, 4);

  EXPECT_FLOAT_EQ(movable_object.direction_x(), 0.6f);
  EXPECT_FLOAT_EQ(movable_object.direction_y(), 0.8f);
}

TEST(MovableObjectTest, SetDirectionRelative) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, /*hit_box_center=*/std::make_pair(1, 2));

  movable_object.SetDirectionRelative(1, 4);

  EXPECT_FLOAT_EQ(movable_object.direction_x(), 0.0f);
  EXPECT_FLOAT_EQ(movable_object.direction_y(), 1.0f);
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

TEST(MovableObjectTest, NoDirectionSet) {
  DummyMovableObject movable_object = DummyMovableObject(
      /*velocity=*/5, std::vector<std::pair<float, float>>{{0, 0}, {0, 2}});

  movable_object.Move();

  EXPECT_EQ(movable_object.center(), (WorldPosition{.x = 0, .y = 1}));
}

TEST(MovableObjectTest, UpdateWorks) {
  DummyMovableObject movable_object_1 = DummyMovableObject(
      /*velocity=*/5, std::vector<std::pair<float, float>>{{1, 0}, {5, 0}});
  std::unique_ptr<DummyMovableObject> movable_object_2 =
      std::make_unique<DummyMovableObject>(
          /*velocity=*/5,
          std::vector<std::pair<float, float>>{{2, -1}, {4, 2}});
  std::list<std::unique_ptr<Object>> objs;
  objs.push_back(std::move(movable_object_2));

  movable_object_1.Update(objs);

  EXPECT_TRUE(movable_object_1.deleted());
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib
