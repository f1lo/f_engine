#ifndef LIP_API_FACTORIES_H
#define LIP_API_FACTORIES_H

#include <memory>

#include "lib/api/objects/object_type.h"
#include "lib/api/sprites/sprite_factory.h"
#include "lib/api/text/font_factory.h"

namespace lib {
namespace api {

struct Factories {
  sprites::SpriteFactory sprite;
  objects::ObjectTypeFactory object_type;
  text::FontFactory font;

  Factories(sprites::SpriteFactory&& s, objects::ObjectTypeFactory&& o,
            text::FontFactory f)
      : sprite(std::move(s)), object_type(std::move(o)), font(std::move(f)) {}

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
