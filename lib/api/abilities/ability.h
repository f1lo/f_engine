#ifndef LIB_API_ABILITIES_ABILITY_H
#define LIB_API_ABILITIES_ABILITY_H

#include "lib/api/abilities/keys.h"
#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"
#include "raylib/include/raylib.h"

namespace lib {
namespace api {
namespace abilities {

class Ability {
 public:
  struct AbilityOpts {
    uint32_t cooldown_sec;
  };

  explicit Ability(const AbilityOpts opts) : opts_(opts) {}
  virtual ~Ability() = default;

  virtual void MaybeUseModifyUser(objects::Object& user) = 0;

  void update_last_used() { last_used_sec_ = GetTime(); }

 protected:
  [[nodiscard]] bool IsOnCooldown() const;

 private:
  AbilityOpts opts_;
  double last_used_sec_ = 0;
};

class MoveAbility : public Ability {
 public:
  struct MoveAbilityOpts : AbilityOpts {
    MoveAbilityOpts(AbilityOpts opts, bool should_hold, double velocity_x,
                    double velocity_y, Button key_left, Button key_right,
                    Button key_top, Button key_bottom)
        : AbilityOpts(opts),
          should_hold(should_hold),
          velocity_x(velocity_x),
          velocity_y(velocity_y),
          key_left(key_left),
          key_right(key_right),
          key_top(key_top),
          key_bottom(key_bottom) {}
    bool should_hold;
    double velocity_x;
    double velocity_y;
    Button key_left;
    Button key_right;
    Button key_top;
    Button key_bottom;
  };
  explicit MoveAbility(const MoveAbilityOpts& opts)
      : Ability({opts.cooldown_sec}), opts_(opts) {}

  void MaybeUseModifyUser(objects::Object& user) override;

 private:
  MoveAbilityOpts opts_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_ABILITY_H