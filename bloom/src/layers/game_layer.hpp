/**
 * @file game_layer.hpp
 * @date 20/12/2024
 *
 * @brief Vulkan layer for the game
 *
 * This file contains the @c GameLayer class, which represents a Vulkan layer used to render 3D game elements.
 * The @c GameLayer class is responsible for managing the lifecycle of the layer, including attaching, beginning,
 * ticking, rendering, detaching, and handling events.
 */

#pragma once
#include "layer.hpp"
#include "src/factory.hpp"
#include "src/render/descriptor_layouts.hpp"
#include "src/render/systems/simple_render_system.hpp"

namespace bloom::render {

/**
 * @class GameLayer
 * @brief Represents a Vulkan layer for rendering 3D game elements.
 *
 * The @c GameLayer class  inherits from the `Layer` class. It provides the functionality for the 3D rendering and
 * handling of game elements
 */
class GameLayer final : public Layer {

public:
  explicit GameLayer(const std::string& name = "Game") : Layer(name) {}

  // Game loop
  void OnAttach() override;
  void OnBegin() override;
  void OnTick(float deltaTime) override;
  void OnRender(FrameInfo frameInfo) override;
  void OnDetach() override;
  void OnEvent(Event& e) override;

private:
  std::unique_ptr<Factory> m_factory = nullptr;       ///< Factory for creating game objects.
  SimpleRenderSystem* m_simpleRenderSystem = nullptr; ///< Rendering system

  std::unique_ptr<DescriptorPool> m_globalPool;
  std::unique_ptr<DescriptorPool> m_materialPool;
  std::vector<VkDescriptorSet> m_globalDescriptorSets;
  std::vector<VkDescriptorSet> m_materialDescriptorSets;
  std::unique_ptr<DescriptorSetLayout> m_globalSetLayout;
  std::unique_ptr<DescriptorSetLayout> m_materialSetLayout;
  std::vector<Buffer*> m_UBOBuffers;
};

}