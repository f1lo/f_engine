#ifndef LIB_API_STATS_H
#define LIB_API_STATS_H

#include <optional>

#include "absl/container/flat_hash_map.h"
#include "absl/time/time.h"
#include "lib/api/objects/object_type.h"

namespace lib {
namespace api {

class Stats {
 public:
  void AddCollision(objects::ObjectType type_1, objects::ObjectType type_2);
  std::optional<absl::Time> GetLastCollisionTime(objects::ObjectType type_1,
                                                 objects::ObjectType type_2);
  int GetCollisionCount(objects::ObjectType type_1, objects::ObjectType type_2);

 private:
  friend class Game;
  friend class StatsTest;
  Stats() = default;

  struct CollisionData {
    int collision_count = 0;
    absl::Time last_collision_time;
  };

  absl::flat_hash_map<std::pair<objects::ObjectType, objects::ObjectType>,
                      CollisionData>
      collisions_;
};

}  // namespace api
}  // namespace lib

#endif  //LIB_API_STATS_H
