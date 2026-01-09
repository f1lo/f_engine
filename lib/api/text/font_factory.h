#ifndef LIB_API_TEXT_FONT_FACTORY_H
#define LIB_API_TEXT_FONT_FACTORY_H

#include <string>

#include "absl/container/flat_hash_map.h"
#include "lib/api/text/f_font.h"

namespace lib {
namespace api {

class Game;

namespace text {

class FontFactory {
 public:
  FontFactory(FontFactory&&) = default;
  FontFactory& operator=(FontFactory&&) = default;
  // Delete copy operations.
  FontFactory(const FontFactory&) = default;
  FontFactory& operator=(const FontFactory&) = default;

 private:
  friend class lib::api::Game;
  FontFactory() = default;

  absl::flat_hash_map<std::string, std::unique_ptr<FFont>> fonts_;
};

}  // namespace text
}  // namespace api
}  // namespace lib

#endif  // LIB_API_TEXT_FONT_FACTORY_H
