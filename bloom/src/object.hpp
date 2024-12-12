/**
 * @file object.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 10/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "render/model.hpp"
#include "render/texture.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <bloom_header.hpp>

namespace bloom {

struct BLOOM_API Transform {
  glm::vec3 position = {};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};
  glm::vec3 rotation = {};

  glm::mat4 mat4() {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{
    {
      scale.x * (c1 * c3 + s1 * s2 * s3),
      scale.x * (c2 * s3),
      scale.x * (c1 * s2 * s3 - c3 * s1),
      0.0f,
    },
    {
      scale.y * (c3 * s1 * s2 - c1 * s3),
      scale.y * (c2 * c3),
      scale.y * (c1 * c3 * s2 + s1 * s3),
      0.0f,
    },
    {
      scale.z * (c2 * s1),
      scale.z * (-s2),
      scale.z * (c1 * c2),
      0.0f,
    },
    {position.x, position.y, position.z, 1.0f}};
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

  render::Texture* texture;

  id_t GetID() const { return m_id; }
private:
  const id_t m_id;
};

}
