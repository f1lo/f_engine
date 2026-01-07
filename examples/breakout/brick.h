#ifndef EXAMPLES_BREAKOUT_BRICK_H
#define EXAMPLES_BREAKOUT_BRICK_H

#include "lib/api/objects/object_type.h"
#include "lib/api/objects/static_object.h"

namespace breakout {

class BrickObject : public lib::api::objects::StaticObject {
 public:
  BrickObject(lib::api::objects::ObjectType type, StaticObjectOpts options,
              const std::vector<std::pair<float, float>>& hit_box_vertices)
      : StaticObject(type, options, hit_box_vertices) {}
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BRICK_H