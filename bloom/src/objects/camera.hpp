/**
 * @file camera.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 12/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "object.hpp"

// thx windows.h i hate you -x
#undef near
#undef far

namespace bloom {

class BLOOM_API Camera : public Object {
public:
  Camera(id_t id) : Object(id) {};

  void Begin() override;
  void Tick(float deltaTime) override;
  void End() override;

  void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
  void SetPerspectiveProjection(float fov, float aspect, float near, float far);

  void UpdateView();

  const glm::mat4& GetProjection() const { return m_projectionMatrix; }
  const glm::mat4& GetView() const { return m_viewMatrix; }

private:
  glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
  glm::mat4 m_viewMatrix = glm::mat4(1.0f);

  // TODO: Implement this with event window resize
  float m_aspect = 1.0f;

  float m_rotation = 0.0f;
};

}
