#include <iostream>
#include <filesystem>
#include <tuple>

#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "config/globals.h"
#include "utils/shader.h"
#include "utils/gl_util.h"

static void ProcessInput(GLFWwindow* window);
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static std::tuple<std::string, std::string> GetShaderPaths();
static std::tuple<std::string, std::string> GetTexturePaths();

static int window_width = 800;
static int window_height = 600;

static float texture2_ratio = 0.2f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Hello Window", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetScrollCallback(window, ScrollCallback);
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
    // 顶点三个值       |  纹理坐标两个值
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left
  };

  unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  glEnable(GL_DEPTH_TEST);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  // 5个值里前3个值为位置信息
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture attribute
  // 5个值里面后2个为纹理坐标，所以偏移量为3
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  auto [container_path, face_path] = GetTexturePaths();

  // OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。stb_image.h能够在图像加载时翻转y轴
  GLuint texture1 = utils::LoadTexture(container_path, GL_RGB, GL_RGB, true);
  GLuint texture2 = utils::LoadTexture(face_path, GL_RGBA, GL_RGBA, true);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (glfwWindowShouldClose(window) == GL_FALSE) {
    glfwPollEvents();

    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    // 设置片段着色器中采样器使用的纹理单元
    shader.SetInt("texture1", 0);
    shader.SetInt("texture2", 1);
    shader.SetFloat("texture2Ratio", texture2_ratio);

    // 根据纹理单元绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);

  glfwTerminate();
  return 0;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    texture2_ratio += 0.05f;
  } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    texture2_ratio -= 0.05f;
  }
  std::cout << texture2_ratio << std::endl;
}

static void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
  texture2_ratio += y_offset / 100;
  std::cout << texture2_ratio << std::endl;
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);
}

static std::tuple<std::string, std::string> GetShaderPaths() {
  std::filesystem::path path(__FILE__);
  return {
    path.parent_path().append("1.2texture.vs").string(),
    path.parent_path().append("1.2texture.fs").string(),
  };
}

static std::tuple<std::string, std::string> GetTexturePaths() {
  return {
    std::filesystem::path(RESOURCE_DIR).append("textures").append("container.jpg").string(),
    std::filesystem::path(RESOURCE_DIR).append("textures").append("awesomeface.png").string(),
  };
}
