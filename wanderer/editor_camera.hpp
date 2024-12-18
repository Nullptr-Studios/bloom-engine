/**
 * @file editor_camera.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 15/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "src/objects/camera.hpp"

namespace wanderer {

class EditorCamera : public bloom::Camera {
public:
  EditorCamera(const id_t id, bloom::render::Devices* devices) : Camera(id, devices) {}
  void OnTick(float deltaTime) override;

  void OnEvent(const bloom::Event &e);
private:
  glm::vec2 m_movement = glm::vec2(0.0f);
  float speed = 1.0f;
};

}
