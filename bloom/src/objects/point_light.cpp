#include "point_light.hpp"

namespace bloom {

void PointLight::PropertiesPanel() {
  Object::PropertiesPanel();
  if (ImGui::CollapsingHeader("Point Light")) {
    ImGui::ColorEdit3("Light Color", &m_color.x);
    ImGui::DragFloat("Intensity", &m_intensity);
    ImGui::DragFloat("Radius", &m_radius);
  }
}

PointLightInfo PointLight::GetInfo() {
  PointLightInfo info = {};
  info.position = glm::vec4(transform.position, m_radius);
  info.color = glm::vec4(m_color, m_intensity);
  return info;
}

}
