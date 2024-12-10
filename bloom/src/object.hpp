/**
 * @file object.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 10/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "engine.hpp"
#include "render/renderer.hpp"
#include <bloom_header.hpp>

namespace bloom {

struct BLOOM_API Transform {
  glm::vec2 position;
  glm::vec2 scale = {1.0f, 1.0f};
  float rotation = 0.0f;

  glm::mat2 mat2() {
    const float cos = std::cos(rotation);
    const float sin = std::sin(rotation);
    glm::mat2 rotationMatrix = {
      {cos, sin},
      {-sin, cos}
    };

    glm::mat2 scaleMatrix = {
      {scale.x, 0.0f},
      {0.0f, scale.y}
    };
    return rotationMatrix * scaleMatrix;
  }
};

class BLOOM_API Object {
  using id_t = unsigned int;

public:
  Object(const id_t id) : m_id(id) {};
  ~Object() = default;
    
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;
  Object(Object&&) = default;
  Object& operator=(Object&&) = default;

  Transform transform;
  std::shared_ptr<render::Model> model;
  glm::vec3 color;

private:
  const id_t m_id;
};

}
