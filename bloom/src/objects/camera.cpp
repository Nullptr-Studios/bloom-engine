#include "camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace bloom {

void Camera::OnBegin() {
  transform.position = glm::vec3(0.0f);
  SetPerspectiveProjection();
  UpdateView();
}

void Camera::OnTick(float deltaTime) {
  SetPerspectiveProjection();
  UpdateView();
}

void Camera::OnClose() {}
void Camera::PropertiesPanel() {
  Object::PropertiesPanel();
  if (ImGui::CollapsingHeader("Camera")) {
    ImGui::DragFloat("FOV", &m_fov, 0.1f);
    ImGui::DragFloat("Aspect", &m_aspect, 0.1f);
    ImGui::DragFloat("Near Clip", &m_near, 0.1f);
    ImGui::DragFloat("Far Clip", &m_far, 0.1f);
  }
}

void Camera::SetOrthographicProjection(const float left, const float right,
    const float bottom, const float top, const float near, const float far) {
  m_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::SetPerspectiveProjection() {
  m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

void Camera::UpdateView() {
  const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), transform.rotation.z, glm::vec3(0, 0, 1)) *
                                   glm::rotate(glm::mat4(1.0f), transform.rotation.x, glm::vec3(1, 0, 0)) *
                                   glm::rotate(glm::mat4(1.0f), transform.rotation.y, glm::vec3(0, 1, 0));
  const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -transform.position);
  m_viewMatrix = rotationMatrix * translationMatrix;
}

}
