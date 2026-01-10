#ifndef LIB_INTERNAL_SHADERS_SHADER_INTERNAL_FACTORY
#define LIB_INTERNAL_SHADERS_SHADER_INTERNAL_FACTORY

#include <string_view>

#include "absl/container/flat_hash_map.h"
#include "lib/internal/shaders/shader_internal.h"

namespace lib {
namespace internal {
namespace shaders {

class ShaderInternalFactory {
 public:
  // Delete copy and move operations.
  ShaderInternalFactory(ShaderInternalFactory&&) = delete;
  ShaderInternalFactory& operator=(ShaderInternalFactory&&) = delete;
  ShaderInternalFactory(const ShaderInternalFactory&) = delete;
  ShaderInternalFactory& operator=(const ShaderInternalFactory&) = delete;

  static ShaderInternalFactory& GetInstance();

  const ShaderInternal* MakeVertexShader(std::string_view resource_path);
  const ShaderInternal* MakeFragmentShader(std::string_view resource_path);
  const ShaderInternal* MakeShader(std::string_view vertex_shader_path,
                                   std::string_view fragment_shader_path);
  const ShaderInternal* MakeFontSDF();

 private:
  ShaderInternalFactory() = default;

  ShaderInternal* MakeOrReturn(const ShaderIdInternal& shader_id);

  absl::flat_hash_map<std::string, std::unique_ptr<ShaderInternal>> shaders_;
};

}  // namespace shaders
}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_SHADERS_SHADER_INTERNAL_FACTORY
