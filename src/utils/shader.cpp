#include "utils/shader.h"

#include "spdlog/spdlog.h"
#include "utils/file_util.h"

namespace utils {

namespace {

GLuint LoadShader(const std::string& shader_path, GLenum shader_type) {
  std::string shader_source = utils::ReadFile(shader_path);
  if (shader_source.empty()) {
    return 0;
  }

  GLuint shader = glCreateShader(shader_type);
  const char* source = shader_source.c_str();
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint error_code = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &error_code);
  if (error_code == GL_FALSE) {
    char error_msg[512] = { 0 };
    glGetShaderInfoLog(shader, 512, nullptr, error_msg);
    SPDLOG_ERROR("Failed to compile shader: {}. Error: {}", shader_path, error_msg);
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

}  // namespace

Shader::~Shader() {
  if (program_ != 0) {
    glDeleteProgram(program_);
  }
}

bool Shader::Compile(const std::string& vertex_shader_path, const std::string& fragment_shader_path) {
  GLuint vertex_shader = LoadShader(vertex_shader_path, GL_VERTEX_SHADER);
  if (vertex_shader == 0) {
    return false;
  }

  GLuint fragment_shader = LoadShader(fragment_shader_path, GL_FRAGMENT_SHADER);
  if (fragment_shader == 0) {
    return false;
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vertex_shader);
  glAttachShader(program_, fragment_shader);
  glLinkProgram(program_);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  GLint error_code = 0;
  glGetProgramiv(program_, GL_LINK_STATUS, &error_code);
  if (error_code == GL_FALSE) {
    char error_msg[512] = { 0 };
    glGetProgramInfoLog(program_, 512, nullptr, error_msg);
    SPDLOG_ERROR("Failed to create program. Error: {}", error_msg);
    glDeleteProgram(program_);
    return false;
  }

  return true;
}

void Shader::Use() const {
  glUseProgram(program_);
}

void Shader::SetBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value ? 1 : 0);
}

void Shader::SetInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const {
  glUniform2f(glGetUniformLocation(program_, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(program_, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
  glUniform4fv(glGetUniformLocation(program_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(program_, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const {
  glUniformMatrix2fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
  glUniformMatrix3fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

}  // namespace utils
