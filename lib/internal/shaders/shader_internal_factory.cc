#include "lib/internal/shaders/shader_internal_factory.h"

#include <optional>
#include <string>
#include <string_view>

#include "absl/memory/memory.h"

namespace lib {
namespace internal {
namespace shaders {

namespace {

const std::string kFontSDFPath = "lib/internal/shaders/resources/sdf.fs";

}  // namespace

ShaderInternalFactory& ShaderInternalFactory::GetInstance() {
  static ShaderInternalFactory factory = ShaderInternalFactory();
  return factory;
}

ShaderInternal* ShaderInternalFactory::MakeOrReturn(
    const ShaderIdInternal& shader_id) {
  auto [shader_it, inserted] = shaders_.try_emplace(shader_id.id(), nullptr);
  if (inserted) {
    shader_it->second = absl::WrapUnique(new ShaderInternal(shader_id));
  }

  return shader_it->second.get();
}

const ShaderInternal* ShaderInternalFactory::MakeVertexShader(
    std::string_view resource_path) {
  ShaderIdInternal shader_id =
      ShaderIdInternal(/*vertex_shader_path=*/resource_path,
                       /*fragment_shader_path=*/std::nullopt);

  return MakeOrReturn(shader_id);
}

const ShaderInternal* ShaderInternalFactory::MakeFragmentShader(
    std::string_view resource_path) {
  ShaderIdInternal shader_id =
      ShaderIdInternal(/*vertex_shader_path=*/std::nullopt,
                       /*fragment_shader_path=*/resource_path);

  return MakeOrReturn(shader_id);
}

const ShaderInternal* ShaderInternalFactory::MakeShader(
    std::string_view vertex_shader_path,
    std::string_view fragment_shader_path) {
  ShaderIdInternal shader_id =
      ShaderIdInternal(vertex_shader_path, fragment_shader_path);

  return MakeOrReturn(shader_id);
}

const ShaderInternal* ShaderInternalFactory::MakeFontSDF() {
  return MakeFragmentShader(kFontSDFPath);
}

}  // namespace shaders
}  // namespace internal
}  // namespace lib
