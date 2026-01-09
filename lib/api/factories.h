#ifndef LIP_API_FACTORIES_H
#define LIP_API_FACTORIES_H

#include <memory>

#include "lib/api/objects/object_type.h"
#include "lib/api/sprites/sprite_factory.h"

namespace lib {
namespace api {

struct Factories {
  sprites::SpriteFactory sprite;
  objects::ObjectTypeFactory object_type;

  Factories(sprites::SpriteFactory&& s, objects::ObjectTypeFactory&& o)
      : sprite(std::move(s)), object_type(std::move(o)) {}

  // Delete copy and move operations.
  Factories(const Factories&) = delete;
  Factories& operator=(const Factories&) = delete;
  Factories(Factories&&) = delete;
  Factories& operator=(Factories&&) = delete;

  Factories() = delete;
};

}  // namespace api
}  // namespace lib

#endif  // LIP_API_FACTORIES_H
