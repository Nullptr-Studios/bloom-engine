#include "editor_camera.hpp"
#include "src/events/key_event.hpp"

namespace wanderer {

void EditorCamera::OnBegin() {
  Camera::OnBegin();
}

void EditorCamera::OnTick(float deltaTime) {
  Camera::OnTick(deltaTime);

  transform.position += glm::vec3(m_movement.x, 0.0f, -m_movement.y) * speed * deltaTime;
}

void EditorCamera::OnEvent(bloom::Event &e) {
  Camera::OnEvent(e);

  if (e.GetEventType() == bloom::EventType::KeyPressed) {
    const auto& keyEvent = static_cast<const bloom::KeyPressedEvent&>(e);
    if (keyEvent.GetKeyCode() == 0x57) {
      m_movement.y = 1.0f;
    }
    if (keyEvent.GetKeyCode() == 0x53) {
      m_movement.y = -1.0f;
    }
    if (keyEvent.GetKeyCode() == 0x41) {
      m_movement.x = -1.0f;
    }
    if (keyEvent.GetKeyCode() == 0x44) {
      m_movement.x = 1.0f;
    }
  } else if (e.GetEventType() == bloom::EventType::KeyReleased) {
    const auto& keyEvent = static_cast<const bloom::KeyReleasedEvent&>(e);
    if (keyEvent.GetKeyCode() == 0x57 || keyEvent.GetKeyCode() == 0x53) {
      m_movement.y = 0.0f;
    } else if (keyEvent.GetKeyCode() == 0x41 || keyEvent.GetKeyCode() == 0x44) {
      m_movement.x = 0.0f;
    }
  }
}

}
