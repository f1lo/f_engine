#include "lib/api/objects/object_type.h"

namespace lib {
namespace api {
namespace objects {

namespace {

constexpr uint16_t kPlayer = 0;
constexpr uint16_t kEnemy = 1;
constexpr uint16_t kScreenLeft = 2;
constexpr uint16_t kScreenRight = 3;
constexpr uint16_t kScreenTop = 4;
constexpr uint16_t kScreenBottom = 5;
constexpr uint16_t kMousePointer = 6;
constexpr uint16_t kButton = 7;
constexpr uint16_t kProjectilePlayer = 8;
constexpr uint16_t kCoordinate = 9;
constexpr uint16_t kWorldBorder = 10;
constexpr uint16_t kSpriteBoundingBox = 11;

}  // namespace

ObjectType::ObjectType(const uint16_t type) : type_(type) {}

bool ObjectType::operator==(const ObjectType& other) const {
  return type_ == other.type_;
}

bool ObjectType::operator<(const ObjectType& other) const {
  return type_ < other.type_;
}

bool ObjectType::IsPlayer() const {
  return type_ == kPlayer;
}
bool ObjectType::IsEnemy() const {
  return type_ == kEnemy;
}
bool ObjectType::IsScreenLeft() const {
  return type_ == kScreenLeft;
}
bool ObjectType::IsScreenRight() const {
  return type_ == kScreenRight;
}
bool ObjectType::IsScreenTop() const {
  return type_ == kScreenTop;
}
bool ObjectType::IsScreenBottom() const {
  return type_ == kScreenBottom;
}
bool ObjectType::IsScreenEdge() const {
  return IsScreenTop() || IsScreenBottom() || IsScreenLeft() || IsScreenRight();
}
bool ObjectType::IsMousePointer() const {
  return type_ == kMousePointer;
}
bool ObjectType::IsButton() const {
  return type_ == kButton;
}
bool ObjectType::IsProjectilePlayer() const {
  return type_ == kProjectilePlayer;
}
bool ObjectType::IsCoordinate() const {
  return type_ == kCoordinate;
}
bool ObjectType::IsWorldBorder() const {
  return type_ == kWorldBorder;
}
bool ObjectType::IsSpriteBoundingBox() const {
  return type_ == kSpriteBoundingBox;
}

ObjectType ObjectTypeFactory::MakeNewObjectType() {
  return ObjectType(++free_type_);
}

ObjectType ObjectTypeFactory::MakePlayer() {
  return ObjectType(kPlayer);
}
ObjectType ObjectTypeFactory::MakeEnemy() {
  return ObjectType(kEnemy);
}
ObjectType ObjectTypeFactory::MakeScreenLeft() {
  return ObjectType(kScreenLeft);
}
ObjectType ObjectTypeFactory::MakeScreenRight() {
  return ObjectType(kScreenRight);
}
ObjectType ObjectTypeFactory::MakeScreenTop() {
  return ObjectType(kScreenTop);
}
ObjectType ObjectTypeFactory::MakeScreenBottom() {
  return ObjectType(kScreenBottom);
}
ObjectType ObjectTypeFactory::MakeMousePointer() {
  return ObjectType(kMousePointer);
}
ObjectType ObjectTypeFactory::MakeButton() {
  return ObjectType(kButton);
}
ObjectType ObjectTypeFactory::MakeProjectilePlayer() {
  return ObjectType(kProjectilePlayer);
}
ObjectType ObjectTypeFactory::MakeCoordinate() {
  return ObjectType(kCoordinate);
}
ObjectType ObjectTypeFactory::MakeWorldBorder() {
  return ObjectType(kWorldBorder);
}
ObjectType ObjectTypeFactory::MakeSpriteBoundingBox() {
  return ObjectType(kSpriteBoundingBox);
}

}  // namespace objects
}  // namespace api
}  // namespace lib
