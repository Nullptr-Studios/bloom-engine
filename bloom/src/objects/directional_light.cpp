#include "directional_light.hpp"

namespace bloom {

void DirectionalLight::PropertiesPanel() {
  Object::PropertiesPanel();
  if (ImGui::CollapsingHeader("Directional Light")) {
    ImGui::DragFloat3("Direction", &m_direction.x, 0.1f);
    ImGui::Text("Color doesn't do anything yet");
    ImGui::ColorEdit3("Color", &m_color.r);
    ImGui::DragFloat("Indirect Intensity", &m_indirectIntensity, 0.01f);
  }
}

}
