#ifndef LIB_API_OBJECTS_OBJECT_TYPE_H
#define LIB_API_OBJECTS_OBJECT_TYPE_H

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

  [[nodiscard]] bool IsPlayer() const;
  [[nodiscard]] bool IsEnemy() const;
  [[nodiscard]] bool IsScreenLeft() const;
  [[nodiscard]] bool IsScreenRight() const;
  [[nodiscard]] bool IsScreenTop() const;
  [[nodiscard]] bool IsScreenBottom() const;
  [[nodiscard]] bool IsScreenEdge() const;
  [[nodiscard]] bool IsMousePointer() const;
  [[nodiscard]] bool IsButton() const;
  [[nodiscard]] bool IsProjectilePlayer() const;
  [[nodiscard]] bool IsCoordinate() const;
  [[nodiscard]] bool IsWorldBorder() const;
  [[nodiscard]] bool IsSpriteBoundingBox() const;

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

  ObjectTypeFactory(ObjectTypeFactory&&) = default;
  ObjectTypeFactory& operator=(ObjectTypeFactory&&) = default;
  // Delete copy operations.
  ObjectTypeFactory(const ObjectTypeFactory&) = delete;
  ObjectTypeFactory& operator=(const ObjectTypeFactory&) = delete;

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
