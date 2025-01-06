/**
 * @file point_light.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 06/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "light.hpp"
#include "src/render/frame_info.hpp"

namespace bloom {

class BLOOM_API PointLight final : public Light {
public:
  PointLight(const id_t id) : Light(id) {}

  void PropertiesPanel() override;

  float GetRadius() const { return m_radius; }
  float GetIntensity() const { return m_intensity; }
  glm::vec3 GetColor() const { return m_color; }

  void SetRadius(float radius) { m_radius = radius; }
  void SetIntensity(float intensity) { m_intensity = intensity; }
  void SetColor(const glm::vec3 &color) { m_color = color; }
  PointLightInfo GetInfo();

private:
  const Type m_type = Point;
  float m_radius = 10.0f; ///< The radius of the light
};

/**
 * @typedef PointLightMap
 * @brief A map of all the point lights
 */
typedef std::unordered_map<id_t, std::shared_ptr<PointLight>> PointLightMap;

}
