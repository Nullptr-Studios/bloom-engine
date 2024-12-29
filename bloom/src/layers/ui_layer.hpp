/**
 * @file ui_layer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 21/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "layer.hpp"
#include "src/events/game_event.hpp"
#include "src/events/key_event.hpp"
#include "src/events/mouse_event.hpp"
#include "src/render/descriptors.hpp"

namespace bloom::ui {

class BLOOM_API UILayer : public Layer {
public:
  explicit UILayer(const std::string& name = "UI") : Layer(name) {}

  // Game loop
  void OnAttach() override;
  void OnBegin() override;
  void OnTick(float deltaTime) override;
  void OnRender(render::FrameInfo frameInfo) override;
  void OnDetach() override;
  void OnEvent(Event& e) override;

private:
  bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
  bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
  bool OnMouseMoved(MouseMovedEvent& e);
  bool OnMouseScrolled(MouseScrolledEvent& e);
  bool OnKeyPressed(KeyPressedEvent& e);
  bool OnKeyReleased(KeyReleasedEvent& e);
  bool OnKeyTyped(KeyTypedEvent& e);
  bool OnWindowResized(WindowResizeEvent& e);

  /**
   * @brief Translates a Bloom key code to an ImGui key code.
   * @param keycode The Bloom key code to translate.
   * @return The ImGui key code.
   */
  ImGuiKey TranslateToImGui(int keycode);

  render::Devices* m_device = nullptr;
  std::unique_ptr<render::DescriptorPool> m_imguiPool;
};

}
