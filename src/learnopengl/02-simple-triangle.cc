#include <iostream>
#include "glad/glad.h"
#include "GLFW//glfw3.h"

static void ProcessInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

const char* kVertexShader = "#version 330 core\n"
                            "layout (location = 0) in vec3 Pos;"
                            "void main()"
                            "{ gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0f); }";

const char* kFragmentShader = "#version 330 core\n"
                              "out vec4 fragColor;"
                              "void main()"
                              "{ fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); }";

float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
};

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

  GLint width = 0;
  GLint height = 0;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Compile vertex shader.
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &kVertexShader, nullptr);
  glCompileShader(vertex_shader);

  // Check the status of the compilation of vertex shader.
  int error_code = 0;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &error_code);
  if (error_code == GL_FALSE) {
    char error_msg[512] = { 0 };
    glGetShaderInfoLog(vertex_shader, 512, nullptr, error_msg);
    std::cerr << "Failed to create vertex shader: " << error_msg << std::endl;
    return -1;
  }

  // Compile fragment shader.
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &kFragmentShader, nullptr);
  glCompileShader(fragment_shader);

  // Check the status of the compilation of fragment shader.
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &error_code);
  if (error_code == GL_FALSE) {
    char error_msg[512] = { 0 };
    glGetShaderInfoLog(fragment_shader, 512, nullptr, error_msg);
    std::cerr << "Failed to create fragment shader: " << error_msg << std::endl;
    return -1;
  }

  // Link shader program.
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  // Check the status of the shader linking.
  glGetProgramiv(shader_program, GL_LINK_STATUS, &error_code);
  if (error_code == GL_FALSE) {
    char error_msg[512] = { 0 };
    glGetProgramInfoLog(shader_program, 512, nullptr, error_msg);
    std::cerr << "Failed to link shader program: " << error_msg << std::endl;
    return -1;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  GLuint vao = 0;
  GLuint vbo = 0;
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Link vertex attributes.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (glfwWindowShouldClose(window) == GL_FALSE) {
    glfwPollEvents();
    ProcessInput(window);

    glClearColor(0.2, 0.3, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}
