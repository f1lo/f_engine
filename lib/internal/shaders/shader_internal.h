#ifndef LIB_INTERNAL_SHADERS_SHADER_INTERNAL
#define LIB_INTERNAL_SHADERS_SHADER_INTERNAL

#include "raylib/include/raylib.h"

#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "absl/hash/hash.h"

namespace lib {
namespace internal {
namespace shaders {

class ShaderIdInternal {
 public:
  ShaderIdInternal(std::optional<std::string_view> vertex_shader_path,
                   std::optional<std::string_view> fragment_shader_path);

  friend std::ostream& operator<<(std::ostream& os,
                                  const ShaderIdInternal& shader_id) {
    os << "Shader: (" << shader_id.id_ << ")";
    return os;
  }

  const char* vertex_shader_path() const { return vertex_shader_path_; }
  const char* fragment_shader_path() const { return fragment_shader_path_; }
  std::string id() const { return id_; }

  template <typename H>
  friend H AbslHashValue(H h, const ShaderIdInternal& t) {
    return H::combine(std::move(h), t.id());
  }

 private:
  const char* vertex_shader_path_;
  const char* fragment_shader_path_;

  std::string vertex_shader_path_storage_;
  std::string fragment_shader_path_storage_;

  std::string id_;
};

class ShaderInternal {
 public:
  ~ShaderInternal();

  void Activate() const;
  static void Deactivate();

  friend std::ostream& operator<<(std::ostream& os,
                                  const ShaderInternal& shader) {
    os << shader.shader_id_;
    return os;
  }

 private:
  friend class ShaderInternalFactory;

  ShaderInternal(const ShaderIdInternal& shader_id);

  // This is fine because everything runs on a single thread.
  static inline std::string last_activated_shader_ = "";

  ShaderIdInternal shader_id_;
  const Shader shader_;
};

}  // namespace shaders
}  // namespace internal
}  // namespace lib

#endif  // LIB_INTERNAL_SHADERS_SHADER_INTERNAL
