#ifndef LIB_INTERNAL_OBJECT_H
#define LIB_INTERNAL_OBJECT_H

namespace lib {
namespace internal {

class Object {
public:
  virtual ~Object() = default;

  virtual void Move(int pos_x, int pos_y) = 0;
  virtual void Draw() = 0;
};
} // namespace internal
} // namespace lib

#endif // LIB_INTERNAL_OBJECT_H
