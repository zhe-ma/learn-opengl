#include <iostream>
#include "glad/glad.h"
#include "GLFW//glfw3.h"

static void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GL_FALSE) {
    std::cerr << "Failed to create load GL" << std::endl;
    glfwTerminate();
    return -2;
  }

  glViewport(0, 0, 800, 600);

  while (glfwWindowShouldClose(window) == GL_FALSE) {
    glfwPollEvents();
    ProcessInput(window);

    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
