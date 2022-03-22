#include <iostream>
#include <filesystem>
#include <tuple>
#include <vector>

#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "config/globals.h"
#include "utils/shader.h"
#include "utils/gl_util.h"
#include "utils/fps_camera.h"

static void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void ProcessInput(GLFWwindow *window);
static void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);

static std::tuple<std::string, std::string> GetShaderPaths();
static std::tuple<std::string, std::string> GetTexturePaths();

static int window_width = 800;
static int window_height = 600;

static utils::FpsCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float delta_time = 0.0f;
static float last_time = 0.0f;

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
  glfwSetScrollCallback(window, ScrollCallback);
  glfwSetCursorPosCallback(window, MouseCallback);
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
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
  };

  std::vector<glm::vec3> cube_positions = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
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

    auto current_time = static_cast<float>(glfwGetTime());
    delta_time = current_time - last_time;
    last_time = current_time;

    ProcessInput(window);

    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();

    // 设置片段着色器中采样器使用的纹理单元
    shader.SetInt("texture1", 0);
    shader.SetInt("texture2", 1);

    // 根据纹理单元绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()), (float)window_width / (float)window_height,
                                            0.1f, 100.0f);

    shader.SetMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader.SetMat4("view", view);

    glBindVertexArray(vao);
    for (int i = 0; i < cube_positions.size(); i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cube_positions[i]);
      model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.SetMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);

  glfwTerminate();
  return 0;
}

static void ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessKeyboard(utils::FpsCamera::Movement::FORWARD, delta_time);
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.ProcessKeyboard(utils::FpsCamera::Movement::BACKWARD, delta_time);
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.ProcessKeyboard(utils::FpsCamera::Movement::LEFT, delta_time);
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.ProcessKeyboard(utils::FpsCamera::Movement::RIGHT, delta_time);
  }
}

static void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
  camera.ProcessMouseScroll(static_cast<float>(y_offset));
}

static void MouseCallback(GLFWwindow* window, double x_pos, double y_pos) {
  static bool first_mouse = true;
  static float last_x = 0;
  static float last_y = 0;

  if (first_mouse) {
    last_x = x_pos;
    last_y = y_pos;
    first_mouse = false;
  }

  float x_offset = x_pos - last_x;
  float y_offset = last_y - y_pos;

  last_x = x_pos;
  last_y = y_pos;

  camera.ProcessMouseMovement(x_offset, y_offset);
}

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);
}

static std::tuple<std::string, std::string> GetShaderPaths() {
  std::filesystem::path path(__FILE__);
  return {
    path.parent_path().append("1.3fps_camera.vs").string(),
    path.parent_path().append("1.3fps_camera.fs").string(),
  };
}

static std::tuple<std::string, std::string> GetTexturePaths() {
  return {
    std::filesystem::path(RESOURCE_DIR).append("textures").append("container.jpg").string(),
    std::filesystem::path(RESOURCE_DIR).append("textures").append("awesomeface.png").string(),
  };
}
