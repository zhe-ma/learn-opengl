#include <iostream>
#include <filesystem>
#include <tuple>

#include "glad/glad.h"
#include "GLFW//glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "config/globals.h"
#include "utils/shader.h"
#include "utils/gl_util.h"

//using fs = std::filesystem;

static void ProcessInput(GLFWwindow* window);
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static std::tuple<std::string, std::string> GetShaderPaths();

static int window_width = 800;
static int window_height = 600;

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
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//    0.5f, 0.5f, -0.5f, 1.0f, 1.f,
//    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
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

  std::cout << RESOURCE_DIR << std::endl;
  std::string image_path = std::filesystem::path(RESOURCE_DIR).append("textures").append("awesomeface.png").string();
  GLuint texture = utils::LoadTexture(image_path, false);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (glfwWindowShouldClose(window) == GL_FALSE) {
    glfwPollEvents();
    ProcessInput(window);

    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader.Use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window_width / (float)window_height,
                                            0.1f, 100.0f);

    shader.SetMat4("projection", projection);

    float radius = 5.0f;
//    double camera_angle = glfwGetTime();
    double camera_angle = 0;
    auto camera_x = static_cast<float>(radius * sin(camera_angle));
    auto camera_z = static_cast<float>(radius * cos(camera_angle));

    // 相机位于x-z平面glm::vec3(camera_x, 0.0f, camera_z)，
    // 目标位置位于原点glm::vec3(0.0f, 0.0f, 0.0f)
    // up方向朝上glm::vec3(0.0f, 1.0f, 0.0f)
    // 现在用的透视投影，如果相机位于glm::vec3(0.0f, 0.0f, 2.0f)，就是站在z轴2.0的位置向原点看，看到的东西就是投影的东西
    glm::mat4 view = glm::lookAt(glm::vec3(camera_x, 0.0f, camera_z),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    shader.SetMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
//    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    shader.SetMat4("model", model);


    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

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
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);
}

static std::tuple<std::string, std::string> GetShaderPaths() {
  std::filesystem::path path(__FILE__);
  return {
    path.parent_path().append("1.2fps_camera.vs").string(),
    path.parent_path().append("1.2fps_camera.fs").string(),
  };
}
