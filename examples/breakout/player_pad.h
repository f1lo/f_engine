#ifndef EXAMPLES_BREAKOUT_PLAYER_PAD_H
#define EXAMPLES_BREAKOUT_PLAYER_PAD_H

#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object_type.h"

namespace breakout {

constexpr float kPadOffset = 10;
constexpr float kBallOffset = 3;

class PlayerPad final : public lib::api::objects::MovableObject {
 public:
  PlayerPad(const float screen_width, const float screen_height,
            const float player_width, const float player_height,
            const lib::api::objects::ObjectType ball_type,
            const MovableObjectOpts& options)
      : MovableObject(lib::api::objects::ObjectTypeFactory::MakePlayer(),
                      options,
                      {{
                           (screen_width - player_width) / 2.0,
                           screen_height - kPadOffset,
                       },
                       {(screen_width - player_width) / 2.0,
                        screen_height - kPadOffset - player_height},
                       {(screen_width + player_width) / 2.0,
                        screen_height - kPadOffset - player_height},
                       {(screen_width + player_width) / 2.0,
                        screen_height - kPadOffset}}),
        screen_width_(screen_width),
        screen_height_(screen_height),
        player_width_(player_width),
        player_height_(player_height),
        ball_type_(ball_type) {}

 protected:
  bool OnCollisionCallback(Object& other_object) override;

 private:
  const float screen_width_;
  const float screen_height_;
  const float player_width_;
  const float player_height_;
  const lib::api::objects::ObjectType ball_type_;
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_PLAYER_PAD_H
