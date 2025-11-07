#ifndef LIB_API_ABILITIES_ABILITY_H
#define LIB_API_ABILITIES_ABILITY_H

#include <list>
#include <memory>

#include "lib/api/abilities/controls.h"
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

  Ability(std::unique_ptr<const ControlsInterface> controls,
          const AbilityOpts opts)
      : controls_(std::move(controls)), user_(nullptr), opts_(opts) {}
  virtual ~Ability() = default;

  virtual std::list<ObjectAndAbilities> Use(const Camera& camera) = 0;
  void set_user(objects::Object* user) { user_ = user; }
  objects::Object* user() { return user_; }

 protected:
  template <typename LevelT>
  friend class lib::api::LevelBuilder;
  [[nodiscard]] bool IsOnCooldown() const;
  // TODO(f1lo): Temporary hack, switch to absl::Time.
  double last_used_sec_ = -100;
  std::unique_ptr<const ControlsInterface> controls_;

 private:
  // Does not take ownership.
  objects::Object* user_;
  AbilityOpts opts_;
};

class MoveAbility : public Ability {
 public:
  struct MoveAbilityOpts {
    std::optional<Button> key_left;
    std::optional<Button> key_right;
    std::optional<Button> key_top;
    std::optional<Button> key_bottom;
  };
  explicit MoveAbility(std::unique_ptr<const ControlsInterface> controls,
                       const MoveAbilityOpts& opts)
      : Ability(std::move(controls), {.cooldown_sec = 0}),
        key_left_(opts.key_left),
        key_right_(opts.key_right),
        key_top_(opts.key_top),
        key_bottom_(opts.key_bottom) {}
  ~MoveAbility() override = default;

  [[nodiscard]] std::list<ObjectAndAbilities> Use(
      const Camera& camera) override;

 private:
  std::optional<Button> key_left_;
  std::optional<Button> key_right_;
  std::optional<Button> key_top_;
  std::optional<Button> key_bottom_;
  bool was_used_last_frame_ = false;
};

}  // namespace abilities
}  // namespace api
}  // namespace lib

#endif  // LIB_API_ABILITIES_ABILITY_H
