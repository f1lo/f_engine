#ifndef LIB_API_ABILITIES_ABILITY_H
#define LIB_API_ABILITIES_ABILITY_H

#include <memory>
#include <optional>

#include "lib/api/objects/movable_object.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace abilities {

class Ability {
 public:
  struct AbilityOpts {
    uint32_t cooldown_sec;
  };

  explicit Ability(AbilityOpts opts) : opts_(opts) {}
  virtual ~Ability() = default;

  virtual std::pair<std::optional<objects::Object::PendingUpdate>,
                    std::unique_ptr<objects::MovableObject>>
  MaybeUse() = 0;

 protected:
  bool IsOnCooldown() const;

 private:
  AbilityOpts opts_;
  int last_used_sec_ = 0;
};

class MoveAbility : public Ability {
 public:
  MoveAbility(const AbilityOpts opts, int bound_key, int velocity_x,
              int velocity_y)
      : Ability(opts),
        bound_key_(bound_key),
        velocity_x_(velocity_x),
        velocity_y_(velocity_y) {}

  std::pair<std::optional<objects::Object::PendingUpdate>,
            std::unique_ptr<objects::MovableObject>>
  MaybeUse() override;

 private:
  int bound_key_;
  int velocity_x_;
  int velocity_y_;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_ABILITY_H