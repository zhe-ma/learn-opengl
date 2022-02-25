#include <iostream>
#include <filesystem>
#include <tuple>

#include "glad/glad.h"
#include "GLFW//glfw3.h"

#include "utils/shader.h"

//using fs = std::filesystem;

static void ProcessInput(GLFWwindow* window);
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static std::tuple<std::string, std::string> GetShaderPaths();

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GL_FALSE) {
    std::cerr << "Failed to create load GL" << std::endl;
    glfwTerminate();
    return -2;
  }

  utils::Shader shader;
  auto [vertex_shader_path, fragment_shader_path] = GetShaderPaths();
  if (!shader.Compile(vertex_shader_path, fragment_shader_path)) {
    return -1;
  }

  float vertices[] = {
    // 顶点三个值       |  颜色四个值
    // 顶面黑色
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,

    // 底面白色
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f,

    // 左侧面红色
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,

    // 右侧面绿色
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,

    // 前面蓝色
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,

    // 后面黄色
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f,
  };

  glEnable(GL_DEPTH_TEST);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  // 七个值里前三个值为位置信息
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute
  // 七个值里面后四个为颜色信息，所以偏移量为3
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (glfwWindowShouldClose(window) == GL_FALSE) {
    glfwPollEvents();
    ProcessInput(window);

    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

static void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

static std::tuple<std::string, std::string> GetShaderPaths() {
  std::filesystem::path path(__FILE__);
  return {
    path.parent_path().append("1.1camera_circle.vs").string(),
    path.parent_path().append("1.1camera_circle.fs").string(),
  };
}
