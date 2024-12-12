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

// thx windows.h i hate you -x
#undef near
#undef far

namespace bloom {

class Camera {

public:
  void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
  void SetPerspectiveProjection(float fov, float aspect, float near, float far);

  void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
  void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
  void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

  const glm::mat4& GetProjection() const { return m_projectionMatrix; }
  const glm::mat4& GetView() const { return m_viewMatrix; }
    
private:
  glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
  glm::mat4 m_viewMatrix = glm::mat4(1.0f);
};

}
