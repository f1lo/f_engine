#ifndef EXAMPLES_BREAKOUT_BRICK_H
#define EXAMPLES_BREAKOUT_BRICK_H

#include "lib/api/common_types.h"
#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace breakout {

class BrickObject : public lib::api::objects::StaticObject {
 public:
  BrickObject(lib::api::objects::ObjectType type, StaticObjectOpts options,
              const lib::api::FRectangle rect)
      : StaticObject(type, options, rect) {}
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BRICK_H