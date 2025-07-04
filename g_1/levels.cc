#include "g_1/levels.h"

#include "lib/api/level.h"
#include "lib/api/objects/static_object.h"
#include "lib/api/title_screen_level.h"
#include "opening_level.h"

using lib::api::kTitleScreenLevel;
using lib::api::Level;
using lib::api::objects::StaticObject;
using lib::internal::HitBox;

constexpr int kButtonOffsetX = 300;
constexpr int kButtonOffsetY = 500;
constexpr int kButtonLengthX = 400;
constexpr int kButtonLengthY = 100;
constexpr int kOffsetBetweenButtons = 50;

namespace g_1 {
namespace {}  // namespace

std::unique_ptr<Level> MakeOpeningLevel(bool debug_mode) {
  return std::make_unique<OpeningLevel>(lib::api::kFirstLevel);
}

std::unique_ptr<Level> MakeTitleScreenLevel(bool debug_mode) {
  std::unique_ptr<StaticObject> start_button = std::make_unique<StaticObject>(
      /*kind=*/kButton,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/true,
          /*should_draw_hit_box=*/debug_mode),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{kButtonOffsetX, kButtonOffsetY},
           {kButtonOffsetX, kButtonOffsetY + kButtonLengthY},
           {kButtonOffsetX + kButtonLengthX, kButtonOffsetY},
           {kButtonOffsetX + kButtonLengthX,
            kButtonOffsetY + kButtonLengthY}}));
  std::unique_ptr<StaticObject> exit_button = std::make_unique<StaticObject>(
      /*kind=*/kButton,
      StaticObject::StaticObjectOpts(
          /*is_hit_box_active=*/true,
          /*should_draw_hit_box=*/debug_mode),
      /*hit_box_vertices=*/
      std::vector<std::pair<double, double>>(
          {{kButtonOffsetX,
            kButtonOffsetY + kButtonLengthY + kOffsetBetweenButtons},
           {kButtonOffsetX,
            kButtonOffsetY + 2 * kButtonLengthY + kOffsetBetweenButtons},
           {kButtonOffsetX + kButtonLengthX,
            kButtonOffsetY + kButtonLengthY + kOffsetBetweenButtons},
           {kButtonOffsetX + kButtonLengthX,
            kButtonOffsetY + 2 * kButtonLengthY + kOffsetBetweenButtons}}));

  return std::make_unique<lib::api::TitleScreenLevel>(
      kTitleScreenLevel, std::move(start_button), std::move(exit_button));
}

}  // namespace g_1
