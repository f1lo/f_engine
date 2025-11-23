#ifndef LIB_API_OBJECTS_OBJECT_TYPE_H
#define LIB_API_OBJECTS_OBJECT_TYPE_H

#include <cstdint>

#include "absl/hash/hash.h"

#include "gtest/gtest_prod.h"

namespace lib {
namespace api {

class Game;
class TitleScreenLevelTest;
class StatsTest;

namespace objects {

class ObjectType {
 public:
  bool operator==(const ObjectType& other) const;
  bool operator<(const ObjectType& other) const;

  template <typename H>
  friend H AbslHashValue(H h, const ObjectType& t) {
    return H::combine(std::move(h), t.type_);
  }

  bool IsPlayer() const;
  bool IsEnemy() const;
  bool IsScreenLeft() const;
  bool IsScreenRight() const;
  bool IsScreenTop() const;
  bool IsScreenBottom() const;
  bool IsScreenEdge() const;
  bool IsMousePointer() const;
  bool IsButton() const;
  bool IsProjectilePlayer() const;
  bool IsCoordinate() const;
  bool IsWorldBorder() const;
  bool IsSpriteBoundingBox() const;

 private:
  friend class ObjectTypeFactory;

  explicit ObjectType(uint16_t type);

  uint16_t type_;
};

class ObjectTypeFactory {
 public:
  ObjectType MakeNewObjectType();

  static ObjectType MakePlayer();
  static ObjectType MakeEnemy();
  static ObjectType MakeScreenLeft();
  static ObjectType MakeScreenRight();
  static ObjectType MakeScreenTop();
  static ObjectType MakeScreenBottom();
  static ObjectType MakeMousePointer();
  static ObjectType MakeButton();
  static ObjectType MakeProjectilePlayer();
  static ObjectType MakeCoordinate();
  static ObjectType MakeWorldBorder();
  static ObjectType MakeSpriteBoundingBox();

 private:
  friend class lib::api::Game;
  friend class lib::api::TitleScreenLevelTest;
  friend class lib::api::StatsTest;
  FRIEND_TEST(ObjectTypeTest, EqualityWorks);
  FRIEND_TEST(ObjectTypeTest, LessWorks);
  ObjectTypeFactory() = default;

  uint16_t free_type_ = 12;
};

}  // namespace objects
}  // namespace api
}  // namespace lib

#endif  // LIB_API_OBJECTS_OBJECT_TYPE_H
