#include "camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace bloom {

void Camera::Begin() {
  transform.position = glm::vec3(0.0f);
  UpdateView();
}

void Camera::Tick(float deltaTime) {
  SetPerspectiveProjection(glm::radians(50.0f), m_aspect, 0.1f, 100.0f);
  UpdateView();
}

void Camera::End() { }

void Camera::SetOrthographicProjection(const float left, const float right,
    const float bottom, const float top, const float near, const float far) {
  m_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::SetPerspectiveProjection(const float fov, const float aspect, const float near, const float far) {
  m_projectionMatrix = glm::perspective(fov, aspect, near, far);
}

void Camera::UpdateView() {
  const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), transform.rotation.z, glm::vec3(0, 0, 1)) *
                                   glm::rotate(glm::mat4(1.0f), transform.rotation.x, glm::vec3(1, 0, 0)) *
                                   glm::rotate(glm::mat4(1.0f), transform.rotation.y, glm::vec3(0, 1, 0));
  const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -transform.position);
  m_viewMatrix = rotationMatrix * translationMatrix;
}

}
