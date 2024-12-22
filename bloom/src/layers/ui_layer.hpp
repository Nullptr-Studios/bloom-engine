/**
 * @file ui_layer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 21/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "layer.hpp"
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
  void OnEvent(const Event& e) override;

private:
  render::Devices* m_device = nullptr;
  std::unique_ptr<render::DescriptorPool> m_imguiPool;
};

}
