#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace utils {

class FpsCamera {
public:
  enum class Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };

  // 欧拉角：pitch俯仰角，yaw偏航角，roll翻滚角
  // 相机的偏航角设置为z轴负方向，在x-z平面-90度
  FpsCamera(const glm::vec3& position, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = -90.0f, float pitch = 0.0f);

  float zoom() const {
    return zoom_;
  }

  glm::mat4 GetViewMatrix();

  void ProcessMouseScroll(float yoffset);

  void ProcessKeyboard(Movement direction, float delta_time);

  void ProcessMouseMovement(float x_offset, float y_offset, bool constrain_pitch = true);

private:
  void UpdateVectors();

private:
  glm::vec3 position_;
  glm::vec3 world_up_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;

  float yaw_ = -90.0f;
  float pitch_ = 0.0f;

  float zoom_ = 45.0f;

  float movement_speed_ = 2.5f;
  float mouse_sensitivity = 0.1f;
};

}  // namespace utils
