#include "camera.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace bloom {

void Camera::SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far) {
  m_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::SetPerspectiveProjection(float fov, float aspect, float near, float far) {
  m_projectionMatrix = glm::perspective(fov, aspect, near, far);
}

void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
  m_viewMatrix = glm::lookAt(position, position + direction, up);
}

void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
  SetViewDirection(position, target - position, up);
}

void Camera::SetViewYXZ(glm::vec3 position, glm::vec3 rotation) {
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1)) *
                             glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)) *
                             glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));

  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -position);

  m_viewMatrix = rotationMatrix * translationMatrix;
}

} // namespace bloom
