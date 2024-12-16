/**
 * @file simple_render_system.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 11/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "objects/actor.hpp"
#include "render/devices.hpp"
#include "render/pipeline.hpp"
#include "objects/camera.hpp"
#include "render/frame_info.hpp"

namespace bloom {

class BLOOM_API SimpleRenderSystem {
public:
  SimpleRenderSystem(render::Devices* devices);
  virtual ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem&) = delete;
  SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

  void Begin(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  void RenderObjects(render::FrameInfo& frameInfo, ActorMap actors);

  constexpr static unsigned int MAX_OBJECTS = 1024;

protected:
  void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void CreatePipeline(VkRenderPass renderPass);

  render::Devices* m_devices = nullptr;
  std::unique_ptr<render::Pipeline> m_pipeline = nullptr;
  VkPipelineLayout m_pipelineLayout;

  void CreateDescriptorPool();
  void CreateDescriptorSets();
  // VkDescriptorSet AllocateDescriptorSet(VkDescriptorSetLayout layout);
};

}
