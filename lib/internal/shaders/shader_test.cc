#include "lib/internal/shaders/shader_internal.h"
#include "lib/internal/shaders/shader_internal_factory.h"

#include <optional>
#include <sstream>

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"

namespace lib {
namespace internal {
namespace shaders {
namespace {

using ::testing::HasSubstr;

class ShaderTest {};

using ShaderDeathTest = ShaderTest;

TEST(ShaderDeathTest, ShaderIdNothingSpecified) {
  EXPECT_DEATH(
      ShaderIdInternal(std::nullopt, std::nullopt),
      HasSubstr("None of vertex_shader_path and fragment_shader_path is set."));
}

TEST(ShaderDeathTest, ShaderNotDeactivated) {
  const ShaderInternal* shader =
      ShaderInternalFactory::GetInstance().MakeShader("vert.vs", "frag.fs");
  shader->Activate();

  EXPECT_DEATH(shader->Activate(),
               HasSubstr("Could not activate Shader: (vert.vs, frag.fs), "
                         "currently active shader: vert.vs, frag.fs"));
}

TEST(ShaderTest, ShaderIdNull) {
  const ShaderIdInternal id = ShaderIdInternal(std::nullopt, "frag.fs");

  EXPECT_EQ(id.vertex_shader_path(), nullptr);
  EXPECT_STREQ(id.fragment_shader_path(), "frag.fs");
}

TEST(ShaderTest, ShaderIdDefaultSubstitution) {
  const ShaderIdInternal id = ShaderIdInternal(std::nullopt, "frag.fs");

  EXPECT_EQ(id.id(), "default, frag.fs");
}

TEST(ShaderTest, MakeVertexShader) {
  const ShaderInternal* shader =
      ShaderInternalFactory::GetInstance().MakeVertexShader("vert.vs");

  std::ostringstream oss;
  oss << *shader;

  EXPECT_EQ(oss.str(), "Shader: (vert.vs, default)");
}

TEST(ShaderTest, MakeFragmentShader) {
  const ShaderInternal* shader =
      ShaderInternalFactory::GetInstance().MakeFragmentShader("frag.fs");

  std::ostringstream oss;
  oss << *shader;

  EXPECT_EQ(oss.str(), "Shader: (default, frag.fs)");
}

TEST(ShaderTest, MakeSameFragmentShaderTwice) {
  ShaderInternalFactory::GetInstance().MakeFragmentShader("frag.fs");

  std::ostringstream oss;
  oss << *ShaderInternalFactory::GetInstance().MakeFragmentShader("frag.fs");

  EXPECT_EQ(oss.str(), "Shader: (default, frag.fs)");
}

TEST(ShaderTest, MakeSameShaderTwice) {
  ShaderInternalFactory::GetInstance().MakeShader("vert.vs", "frag.fs");

  std::ostringstream oss;
  oss << *ShaderInternalFactory::GetInstance().MakeShader("vert.vs", "frag.fs");

  EXPECT_EQ(oss.str(), "Shader: (vert.vs, frag.fs)");
}

}  // namespace
}  // namespace shaders
}  // namespace internal
}  // namespace lib
