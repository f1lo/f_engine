#ifndef LIB_API_OBJECTS_OBJECT_UTILS_H
#define LIB_API_OBJECTS_OBJECT_UTILS_H

#include <list>
#include <memory>
#include <optional>

#include "lib/api/objects/object.h"

namespace lib {
namespace api {
namespace objects {

std::optional<Object::PendingUpdate> ExecuteUpdates(
    const Object& object_to_update,
    const std::list<std::unique_ptr<Object>>& other_objects);

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_UTILS_H
