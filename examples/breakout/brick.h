#ifndef EXAMPLES_BREAKOUT_BRICK_H
#define EXAMPLES_BREAKOUT_BRICK_H

#include "lib/api/objects/static_object.h"

namespace breakout {

class BrickObject : public lib::api::objects::StaticObject {
 public:
  BrickObject(lib::api::objects::Kind kind, StaticObjectOpts options,
              const std::vector<std::pair<double, double>>& hit_box_vertices)
      : StaticObject(kind, options, hit_box_vertices) {}
};

}  // namespace breakout

#endif  // EXAMPLES_BREAKOUT_BRICK_H