#pragma once

#include <string>
#include "glad/glad.h"

namespace utils {

GLuint LoadTexture(const std::string& image_path, GLint internal_format, GLenum format, bool flip_y);

}  // namespace utils
