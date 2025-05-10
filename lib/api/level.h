#ifndef LIB_API_LEVEL_H
#define LIB_API_LEVEL_H

#include <list>
#include <memory>

#include "abilities/ability.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {

class Level {
 public:
  void add_object_and_abilities(
      std::unique_ptr<objects::Object> object,
      std::list<std::unique_ptr<abilities::Ability>> abilities) {
    objects_.push_back(std::move(object));
    abilities_.push_back(std::move(abilities));
  }

  void add_object(std::unique_ptr<objects::Object> object) {
    objects_.push_back(std::move(object));
    abilities_.emplace_back();
  }

  void AddScreenObjects();

  [[nodiscard]] const std::list<std::unique_ptr<objects::Object>>& objects()
      const {
    return objects_;
  }
  void Run();

 private:
  void CleanUpOrDie();

  std::list<std::unique_ptr<objects::Object>> objects_;
  std::list<std::list<std::unique_ptr<abilities::Ability>>> abilities_;
};

}  // namespace api
}  // namespace lib

#endif  // LIB_API_LEVEL_H
