/**
 * @file simple_render_system.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 11/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "object.hpp"
#include "render/devices.hpp"
#include "render/pipeline.hpp"

namespace bloom {

class BLOOM_API SimpleRenderSystem {
public:
  SimpleRenderSystem(render::Devices* devices);
  virtual ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem&) = delete;
  SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

  void Begin(VkRenderPass renderPass);
  void RenderObjects(VkCommandBuffer commandBuffer, std::vector<Object> &objects);

protected:
  void CreatePipelineLayout();
  void CreatePipeline(VkRenderPass renderPass);

  render::Devices* m_devices = nullptr;
  std::unique_ptr<render::Pipeline> m_pipeline = nullptr;
  VkPipelineLayout m_pipelineLayout;
};

}
