/**
 * @file game_layer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 20/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "layer.hpp"
#include "src/factory.hpp"
#include "src/render/descriptor_layouts.hpp"
#include "src/render/systems/simple_render_system.hpp"

namespace bloom::render {

class GameLayer final : public Layer {

public:
  // Game loop
  void OnAttach() override;
  void OnBegin() override;
  void OnTick(float deltaTime) override;
  void OnRender(FrameInfo frameInfo) override;
  void OnDetach() override;

  void OnEvent(const Event& e) override;
    
private:
  std::unique_ptr<Factory> m_factory = nullptr;
  SimpleRenderSystem* m_simpleRenderSystem = nullptr;

  std::unique_ptr<DescriptorPool> m_globalPool;
  std::unique_ptr<DescriptorPool> m_materialPool;
  std::vector<VkDescriptorSet> m_globalDescriptorSets;
  std::vector<VkDescriptorSet> m_materialDescriptorSets;
  std::unique_ptr<DescriptorSetLayout> m_globalSetLayout;
  std::unique_ptr<DescriptorSetLayout> m_materialSetLayout;
  // ReSharper disable once CppInconsistentNaming
  std::vector<Buffer*> m_UBOBuffers;
};

}
