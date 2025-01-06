/**
 * @file light.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 06/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "object.hpp"

namespace bloom {

/**
 * @class Light
 * @brief Represents a light source in the scene
 *
 * This class represents a light source in the scene. It provides information
 * on the type of light and how it should be rendered.
 */
class BLOOM_API Light : public Object {
protected:
  /**
   * @enum Type
   * @brief Enum for the type of light
   * This enum provides information on how to render each light by providing
   * the type of the light
   */
  enum Type {
    Directional, ///< Light that shines in a specific direction
    Point,       ///< Light that shines in a specific location
    Spot         ///< Light that shines in a specific direction and location
  };

public:
  /**
   * @brief Get the type of the light
   * @return Type enum
   */
  Type GetType() const { return m_type; }
protected:
  explicit Light(const id_t id) : Object(id) {};

  const Type m_type = Point;
  float m_intensity = 1.0f; ///< The intensity of the light
  glm::vec3 m_color = glm::vec3(1.0f); ///< The color of the light
};

}
