#include <iostream>

#include "utils/file_util.h"

int main() {
  std::string a = utils::ReadFile("/Users/zhe/Documents/projects/learn-opengl/src/learnopengl/test.cpp");
  std::cout << a;
  return 0;
}