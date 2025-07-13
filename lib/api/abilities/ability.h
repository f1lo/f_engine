#ifndef LIB_API_ABILITIES_ABILITY_H
#define LIB_API_ABILITIES_ABILITY_H

#include "raylib/include/raylib.h"

#include <list>
#include <memory>

#include "lib/api/abilities/keys.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {

template <typename LevelT>
class LevelBuilder;

namespace abilities {

class Ability {
 public:
  struct AbilityOpts {
    uint32_t cooldown_sec;
  };

  explicit Ability(const AbilityOpts opts) : user_(nullptr), opts_(opts) {}
  virtual ~Ability() = default;

  virtual std::list<std::unique_ptr<objects::Object>> Use() = 0;

  void update_last_used() { last_used_sec_ = GetTime(); }

 protected:
  template <typename LevelT>
  friend class LevelBuilder;

  [[nodiscard]] bool IsOnCooldown() const;
  // Does not take ownership.
  objects::Object* user_;

 private:
  AbilityOpts opts_;
  double last_used_sec_ = 0;
};

class MoveAbility : public Ability {
 public:
  struct MoveAbilityOpts : AbilityOpts {
    MoveAbilityOpts(const AbilityOpts opts, const bool should_hold,
                    const std::optional<Button> key_left,
                    const std::optional<Button> key_right,
                    const std::optional<Button> key_top,
                    const std::optional<Button> key_bottom)
        : AbilityOpts(opts),
          should_hold(should_hold),
          key_left(key_left),
          key_right(key_right),
          key_top(key_top),
          key_bottom(key_bottom) {}
    bool should_hold;
    const std::optional<Button> key_left;
    const std::optional<Button> key_right;
    const std::optional<Button> key_top;
    const std::optional<Button> key_bottom;
  };
  explicit MoveAbility(const MoveAbilityOpts& opts)
      : Ability({opts.cooldown_sec}), opts_(opts) {}

  [[nodiscard]] std::list<std::unique_ptr<objects::Object>> Use() override;

 private:
  MoveAbilityOpts opts_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_ABILITY_H
