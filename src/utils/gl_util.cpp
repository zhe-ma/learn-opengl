#include "utils/gl_util.h"

#include "stb/stb_image.h"
#include "spdlog/spdlog.h"

namespace utils {

GLuint LoadTexture(const std::string& image_path, GLint internal_format, GLenum format, bool flip_y) {
  GLuint texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // 设置纹理环绕方式和过滤方式：https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  stbi_set_flip_vertically_on_load(flip_y);

  int width = 0;
  int height = 0;
  int channels_in_file = 0;
  unsigned char* data = stbi_load(image_path.c_str(), &width, & height, &channels_in_file, 0);
  if (data == nullptr) {
    SPDLOG_ERROR("Failed to load image: {}.", image_path);
    glDeleteTextures(1, &texture);
    return 0;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  // 生成多级渐远纹理
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  return texture;
}

}  // namespace utils
