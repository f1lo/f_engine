#include "lib/api/text/font_factory.h"

#include <memory>

#include "absl/log/log.h"
#include "absl/memory/memory.h"
#include "lib/api/text/f_font.h"

namespace lib {
namespace api {
namespace text {

namespace {

const char* kRobotoRegularPath = "lib/api/text/fonts/roboto_regular.ttf";
const char* kRobotoItalicPath = "lib/api/text/fonts/roboto_italic.ttf";
const char* kRobotoBoldPath = "lib/api/text/fonts/roboto_bold.ttf";

}  // namespace

const FFont* FontFactory::MakeFFont(const std::string_view resource_path) {
  auto [f_font_it, inserted] = fonts_.try_emplace(resource_path, nullptr);
  if (inserted) {
    f_font_it->second = absl::WrapUnique(new FFont(resource_path));
  }

  return f_font_it->second.get();
}

const FFont* FontFactory::MakeRoboto(FontStyle style) {
  switch (style) {
    case FontStyle::NORMAL:
      return MakeFFont(kRobotoRegularPath);
    case FontStyle::ITALIC:
      return MakeFFont(kRobotoItalicPath);
    case FontStyle::BOLD:
      return MakeFFont(kRobotoBoldPath);
  }

  LOG(FATAL) << "Invalid font style.";
  return nullptr;
}

}  // namespace text
}  // namespace api
}  // namespace lib
