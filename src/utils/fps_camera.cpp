#include "utils/fps_camera.h"

namespace utils {

FpsCamera::FpsCamera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
    : position_(position)
    , world_up_(up)
    , yaw_(yaw)
    , pitch_(pitch) {
  UpdateVectors();
}

void FpsCamera::UpdateVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front.y = sin(glm::radians(pitch_));
  front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

  front_ = glm::normalize(front);
  right_ = glm::normalize(glm::cross(front_, world_up_));
  up_ = glm::normalize(glm::cross(right_, front_));
}
glm::mat4 FpsCamera::GetViewMatrix() {
  return glm::lookAt(position_, position_ + front_, up_);
}

void FpsCamera::ProcessMouseScroll(float y_offset) {
  zoom_ -= y_offset;
  if (zoom_ < 1.0f) {
    zoom_ = 1.0f;
  }

  if (zoom_ > 45.0f) {
    zoom_ = 45.0f;
  }
}

void FpsCamera::ProcessKeyboard(FpsCamera::Movement direction, float delta_time) {
  // 防止屏幕刷新率的不同导致移动的速度不同，所以用时间来进行加权
  float velocity = movement_speed_ * delta_time;
  if (direction == Movement::FORWARD) {
    position_ += front_ * velocity;
  } else if (direction == Movement::BACKWARD) {
    position_ -= front_ * velocity;
  } else if (direction == Movement::LEFT) {
    position_ -= right_ * velocity;
  } else if (direction == Movement::RIGHT) {
    position_ += right_ * velocity;
  }
}

void FpsCamera::ProcessMouseMovement(float x_offset, float y_offset, bool constrain_pitch) {
  x_offset *= mouse_sensitivity;
  y_offset *= mouse_sensitivity;

  yaw_   += x_offset;
  pitch_ += y_offset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrain_pitch) {
    if (pitch_ > 89.0f) {
      pitch_ = 89.0f;
    }

    if (pitch_ < -89.0f)
      pitch_ = -89.0f;
  }

  UpdateVectors();
}

}  // namespace utils
