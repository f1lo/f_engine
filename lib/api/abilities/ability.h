#ifndef LIB_API_ABILITIES_ABILITY_H
#define LIB_API_ABILITIES_ABILITY_H

#include <list>
#include <memory>

#include "lib/api/abilities/keys.h"
#include "lib/api/camera.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {
class Ability;
}  // namespace abilities

template <typename LevelT>
class LevelBuilder;

typedef std::pair<std::unique_ptr<objects::Object>,
                  std::list<std::unique_ptr<abilities::Ability>>>
    ObjectAndAbilities;

namespace abilities {

class Ability {
 public:
  struct AbilityOpts {
    uint32_t cooldown_sec;
  };

  explicit Ability(const AbilityOpts opts) : user_(nullptr), opts_(opts) {}
  virtual ~Ability() = default;

  virtual std::list<ObjectAndAbilities> Use(const Camera& camera) = 0;
  void set_user(objects::Object* user) { user_ = user; }
  objects::Object* user() { return user_; }

 protected:
  template <typename LevelT>
  friend class lib::api::LevelBuilder;
  [[nodiscard]] bool IsOnCooldown() const;
  double last_used_sec_ = 0;

 private:
  // Does not take ownership.
  objects::Object* user_;
  AbilityOpts opts_;
};

class MoveAbility : public Ability {
 public:
  struct MoveAbilityOpts : AbilityOpts {
    MoveAbilityOpts(const AbilityOpts opts,
                    const std::optional<Button> key_left,
                    const std::optional<Button> key_right,
                    const std::optional<Button> key_top,
                    const std::optional<Button> key_bottom)
        : AbilityOpts(opts),
          key_left(key_left),
          key_right(key_right),
          key_top(key_top),
          key_bottom(key_bottom) {}
    const std::optional<Button> key_left;
    const std::optional<Button> key_right;
    const std::optional<Button> key_top;
    const std::optional<Button> key_bottom;
  };
  explicit MoveAbility(const MoveAbilityOpts& opts)
      : Ability({opts.cooldown_sec}), opts_(opts) {}
  ~MoveAbility() override = default;

  [[nodiscard]] std::list<ObjectAndAbilities> Use(
      const Camera& camera) override;

 private:
  MoveAbilityOpts opts_;
  bool was_used_last_frame_ = false;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_ABILITY_H
