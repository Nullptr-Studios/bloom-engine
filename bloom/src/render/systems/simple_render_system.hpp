/**
 * @file simple_render_system.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 11/12/2024
 *
 * @brief Defines the SimpleRenderSystem class for rendering objects.
 *
 * This file contains the declaration of the SimpleRenderSystem class, which is responsible for
 * managing the rendering of objects in the bloom engine. It handles the creation of pipelines
 * and the rendering process.
 */

#pragma once
#include "src/objects/actor.hpp"
#include "src/render/pipeline.hpp"

namespace bloom::render {
struct FrameInfo;
class Devices;

/**
 * @class SimpleRenderSystem
 * @brief Manages the rendering of objects.
 *
 * The SimpleRenderSystem class is responsible for setting up the rendering pipeline and
 * rendering objects in the bloom engine. It handles the creation of pipeline layouts and
 * pipelines, and provides methods to begin the rendering process and render objects.
 */
class BLOOM_API SimpleRenderSystem {
public:
  SimpleRenderSystem(Devices* devices);
  virtual ~SimpleRenderSystem();

  // Delete copy constructor and copy assignment operator
  SimpleRenderSystem(const SimpleRenderSystem&) = delete;
  SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

  /**
   * @brief Begins the rendering process.
   * @param renderPass The Vulkan render pass.
   * @param globalSetLayout The global descriptor set layout.
   * @param materialSetLayout The material descriptor set layout.
   */
  void Begin(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VkDescriptorSetLayout materialSetLayout);
  /**
   * @brief Renders the actors on the renderable map
   * @param frameInfo Information about the current frame.
   * @param actors Map of actors to render.
   */
  void RenderObjects(const FrameInfo& frameInfo, ActorMap actors) const;
  constexpr static unsigned int MAX_OBJECTS = 1024; ///< Maximum number of objects that can be rendered.

protected:
  /**
   * @brief Creates the pipeline layout.
   * @param globalSetLayout The global descriptor set layout.
   * @param materialSetLayout The material descriptor set layout.
   */
  void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout, VkDescriptorSetLayout materialSetLayout);
  /**
   * @brief Creates the pipeline.
   * @param renderPass The Vulkan render pass.
   */
  void CreatePipeline(VkRenderPass renderPass);

  Devices* m_devices = nullptr;
  std::unique_ptr<Pipeline> m_pipeline = nullptr;
  VkPipelineLayout m_pipelineLayout = nullptr;
};

}
