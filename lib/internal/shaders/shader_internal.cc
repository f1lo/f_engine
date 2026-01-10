#include "raylib/include/raylib.h"

#include "lib/internal/shaders/shader_internal.h"

#include <filesystem>
#include <optional>
#include <string_view>

#include "absl/log/check.h"
#include "absl/log/log.h"
#include "absl/strings/substitute.h"

namespace lib {
namespace internal {
namespace shaders {

ShaderIdInternal::ShaderIdInternal(
    std::optional<std::string_view> vertex_shader_path,
    std::optional<std::string_view> fragment_shader_path)
    : vertex_shader_path_storage_(""), fragment_shader_path_storage_("") {
  CHECK(vertex_shader_path.has_value() || fragment_shader_path.has_value())
      << "None of vertex_shader_path and fragment_shader_path is set.";

  vertex_shader_path_ = nullptr;
  if (vertex_shader_path.has_value()) {
    vertex_shader_path_storage_ =
        std::filesystem::path(*vertex_shader_path).make_preferred().string();
    vertex_shader_path_ = vertex_shader_path_storage_.c_str();
  }

  fragment_shader_path_ = nullptr;
  if (fragment_shader_path.has_value()) {
    fragment_shader_path_storage_ =
        std::filesystem::path(*fragment_shader_path).make_preferred().string();
    fragment_shader_path_ = fragment_shader_path_storage_.c_str();
  }

  id_ = absl::Substitute(
      "$0, $1",
      vertex_shader_path.has_value() ? *vertex_shader_path : "default",
      fragment_shader_path.has_value() ? *fragment_shader_path : "default");
}

ShaderInternal::ShaderInternal(const ShaderIdInternal& shader_id)
    : shader_id_(shader_id),
      shader_(LoadShader(shader_id_.vertex_shader_path(),
                         shader_id_.fragment_shader_path())) {}

ShaderInternal::~ShaderInternal() {
  UnloadShader(shader_);
}

void ShaderInternal::Activate() const {
  CHECK(last_activated_shader_.empty())
      << "Could not activate " << *this
      << ", currently active shader: " << last_activated_shader_;
  last_activated_shader_ = shader_id_.id();
  BeginShaderMode(shader_);
}

void ShaderInternal::Deactivate() {
  if (last_activated_shader_.empty()) {
    LOG(WARNING) << "No active shader to deactivate - doing nothing.";
    return;
  }

  last_activated_shader_ = "";
  EndShaderMode();
}

}  // namespace shaders
}  // namespace internal
}  // namespace lib
