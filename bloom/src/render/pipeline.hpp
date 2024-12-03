/**
 * @file pipeline.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 26/11/2024
 *
 * @brief [File description here]
 */

#pragma once

#include "devices.hpp"
#include "renderer.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

struct PipelineConfiguration {
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class Pipeline {
public:
  Pipeline(Devices& device, const std::string& vertPath, const std::string& fragPath, const PipelineConfiguration& config);
  ~Pipeline();

  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;

  void Bind(VkCommandBuffer commandBuffer);
  static void defaultPipelineConfig(PipelineConfiguration& config);

private:
  static std::vector<char> ReadFile(const std::string& path);
  void CreatePipeline(const std::string& vertPath, const std::string& fragPath, const PipelineConfiguration& config);
  void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

  Devices& _device;
  VkPipeline _graphicsPipeline;
  VkShaderModule _vertShaderModule;
  VkShaderModule _fragShaderModule;
};

}
