#include "directional_light.hpp"

namespace bloom {

DirectionalLightInfo DirectionalLight::GetInfo() {
  DirectionalLightInfo info = {};
  info.direction = glm::vec4(transform.rotation, 0.0f);
  info.color = glm::vec4(m_color, m_intensity);
  return info;
}

}