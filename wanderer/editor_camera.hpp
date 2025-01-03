/**
 * @file editor_camera.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 15/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "src/objects/camera.hpp"
#include <imgui.h>

namespace wanderer {

class EditorCamera : public bloom::Camera {
public:
  explicit EditorCamera(const id_t id) : Camera(id) {}
  void OnBegin() override;
  void OnTick(float deltaTime) override;

  void OnEvent(bloom::Event &e) override;
private:
  glm::vec2 m_movement = glm::vec2(0.0f);
  float speed = 1.0f;
};

}
