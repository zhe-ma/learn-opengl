#pragma once

#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace utils {

class Shader {
public:
  Shader() = default;
  ~Shader();

  bool Compile(const std::string& vertex_shader_path, const std::string& fragment_shader_path);

  void Use() const;

  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;

  void SetVec2(const std::string& name, const glm::vec2& value) const;
  void SetVec2(const std::string& name, float x, float y) const;

  void SetVec3(const std::string& name, const glm::vec3& value) const;
  void SetVec3(const std::string& name, float x, float y, float z) const;

  void SetVec4(const std::string& name, const glm::vec4& value) const;
  void SetVec4(const std::string& name, float x, float y, float z, float w) const;

  void SetMat2(const std::string& name, const glm::mat2& mat) const;
  void SetMat3(const std::string& name, const glm::mat3& mat) const;
  void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
  GLuint program_ = 0;
};

}  // namespace utils
