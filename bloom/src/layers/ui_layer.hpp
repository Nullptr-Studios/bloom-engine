/**
 * @file ui_layer.hpp
 * @author Xein 
 * @date 21/12/2024
 *
 * @brief Layer that contains all the Editor UI
 */

#pragma once
#include <imgui.h>
#include "layer.hpp"

namespace bloom {
  
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;
class WindowResizeEvent;

namespace render {
class Devices;
class DescriptorPool;
}

namespace ui {

  /**
   * @class UILayer
   * @brief This layer contains the render configurations for ImgUI
   * IMGUI is an external library used for the engine editor's UI
   */
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
  // Event Stuff
  bool OnMouseButtonPressed(const MouseButtonPressedEvent & e);
  bool OnMouseButtonReleased(const MouseButtonReleasedEvent & e);
  bool OnMouseMoved(const MouseMovedEvent & e);
  bool OnMouseScrolled(const MouseScrolledEvent & e);
  bool OnKeyPressed(const KeyPressedEvent & e);
  bool OnKeyReleased(const KeyReleasedEvent & e);
  bool OnKeyTyped(const KeyTypedEvent & e);
  bool OnWindowResized(const WindowResizeEvent & e);

  /**
   * @brief Translates a Bloom key code to an ImGui key code.
   * @param keycode The Bloom key code to translate.
   * @return The ImGui key code.
   */
  static ImGuiKey TranslateToImGui(int keycode);

  render::Devices* m_device = nullptr;
  std::unique_ptr<render::DescriptorPool> m_imguiPool;
};

}
}
