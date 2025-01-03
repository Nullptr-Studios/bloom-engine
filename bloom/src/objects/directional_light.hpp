/**
 * @file directional_light.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 03/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "object.hpp"

namespace bloom {

/**
 *  @class DirectionalLight
 *  @brief Represents a directional light source in the game world.
 *
 *  The DirectionalLight class extends the Object class, adding properties specific to
 *  directional light sources. It provides a direction vector, color, and indirect intensity
 *  for the light source. Directional lights are used to simulate sunlight or moonlight in
 *  the game world, casting shadows and illuminating objects from a specific direction.
 */
class BLOOM_API DirectionalLight : public Object {
public:
  explicit DirectionalLight(const id_t id) : Object(id) {};

  void PropertiesPanel() override;

  glm::vec3 GetDirection() const { return m_direction; }
  glm::vec3 GetColor() const { return m_color; }
  float GetIndirectIntensity() const { return m_indirectIntensity; }
private:
  glm::vec3 m_direction = glm::vec3(2.0f, -3.0f, 1.0f);
  glm::vec3 m_color = glm::vec3(1.0f);
  float m_indirectIntensity = 0.05f;
  // TODO: implement color
};

}
