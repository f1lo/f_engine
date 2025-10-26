#ifndef LIB_API_OBJECTS_SCREEN_EDGE_OBJECT_H
#define LIB_API_OBJECTS_SCREEN_EDGE_OBJECT_H

#include <list>
#include <memory>

#include "lib/api/common_types.h"
#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

class ScreenEdgeObject : public Object {
 public:
  static std::unique_ptr<ScreenEdgeObject> MakeTop(double screen_width,
                                                   bool should_draw_hit_box);
  static std::unique_ptr<ScreenEdgeObject> MakeBottom(double screen_width,
                                                      double screen_height,
                                                      bool should_draw_hit_box);
  static std::unique_ptr<ScreenEdgeObject> MakeLeft(double screen_height,
                                                    bool should_draw_hit_box);
  static std::unique_ptr<ScreenEdgeObject> MakeRight(double screen_width,
                                                     double screen_height,
                                                     bool should_draw_hit_box);

  void ReAdjustToScreen(WorldPosition screen_top_left_pos, double screen_width,
                        double screen_height);

 private:
  ScreenEdgeObject(Kind kind, bool should_draw_hit_box, ScreenPosition a,
                   ScreenPosition b);

  void Update(
      const std::list<std::unique_ptr<Object>>& other_objects) override {}
  bool OnCollisionCallback(Object& other_object) override { return false; }
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_SCREEN_EDGE_OBJECT_H
