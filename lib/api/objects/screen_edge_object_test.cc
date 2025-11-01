#include "lib/api/objects/screen_edge_object.h"

#include <memory>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include "lib/api/common_types.h"

namespace lib {
namespace api {
namespace objects {
namespace {

TEST(ScreenEdgeObjectTest, TopAdjustement) {
  std::unique_ptr<ScreenEdgeObject> edge = ScreenEdgeObject::MakeTop(
      /*screen_width=*/100, /*should_draw_hit_box=*/false);

  edge->ReAdjustToScreen(/*screen_top_left_pos=*/{.x = 20, .y = 10},
                         /*screen_width=*/100, /*screen_height=*/50);

  EXPECT_EQ(edge->center(), (WorldPosition{.x = 70, .y = 10}));
}

TEST(ScreenEdgeObjectTest, BottomAdjustement) {
  std::unique_ptr<ScreenEdgeObject> edge = ScreenEdgeObject::MakeBottom(
      /*screen_width=*/100, /*screen_height=*/50,
      /*should_draw_hit_box=*/false);

  edge->ReAdjustToScreen(/*screen_top_left_pos=*/{.x = 20, .y = 10},
                         /*screen_width=*/100, /*screen_height=*/50);

  EXPECT_EQ(edge->center(), (WorldPosition{.x = 70, .y = 60}));
}

TEST(ScreenEdgeObjectTest, LeftAdjustement) {
  std::unique_ptr<ScreenEdgeObject> edge =
      ScreenEdgeObject::MakeLeft(/*screen_height=*/50,
                                 /*should_draw_hit_box=*/false);

  edge->ReAdjustToScreen(/*screen_top_left_pos=*/{.x = 20, .y = 10},
                         /*screen_width=*/100, /*screen_height=*/50);

  EXPECT_EQ(edge->center(), (WorldPosition{.x = 20, .y = 35}));
}

TEST(ScreenEdgeObjectTest, RightAdjustement) {
  std::unique_ptr<ScreenEdgeObject> edge =
      ScreenEdgeObject::MakeRight(/*screen_width=*/100, /*screen_height=*/50,
                                  /*should_draw_hit_box=*/false);

  edge->ReAdjustToScreen(/*screen_top_left_pos=*/{.x = 20, .y = 10},
                         /*screen_width=*/100, /*screen_height=*/50);

  EXPECT_EQ(edge->center(), (WorldPosition{.x = 120, .y = 35}));
}

}  // namespace
}  // namespace objects
}  // namespace api
}  // namespace lib
