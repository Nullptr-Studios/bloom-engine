/**
 * @file pipeline.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 26/11/2024
 *
 * @brief Defines the Pipeline class for managing Vulkan graphics pipelines.
 *
 * This file contains the declaration of the Pipeline class and the PipelineConfiguration struct,
 * which are used to create and manage Vulkan graphics pipelines in the bloom rendering engine.
 */

#pragma once

#include "devices.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

/**
 * @struct PipelineConfiguration
 * @brief Holds configuration settings for creating a Vulkan graphics pipeline.
 *
 * This struct contains various Vulkan pipeline state create info structures and other settings
 * required to configure and create a Vulkan graphics pipeline.
 */
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

/**
 * @class Pipeline
 * @brief Manages a Vulkan graphics pipeline.
 *
 * The Pipeline class encapsulates the creation, management, and binding of a Vulkan graphics pipeline.
 */
class Pipeline {
public:
  /**
   * @brief Constructs a Pipeline instance.
   * @param device Reference to the Devices instance.
   * @param vertPath Path to the vertex shader file.
   * @param fragPath Path to the fragment shader file.
   * @param config Reference to the PipelineConfiguration struct.
   */
  Pipeline(Devices& device, const std::string& vertPath, const std::string& fragPath, const PipelineConfiguration& config);
  ~Pipeline();

  // Copy constructors
  Pipeline(const Pipeline&) = delete;
  Pipeline& operator=(const Pipeline&) = delete;

  /**
   * @brief Binds the pipeline to the command buffer.
   * @param commandBuffer The Vulkan command buffer to bind the pipeline to.
   */
  void Bind(VkCommandBuffer commandBuffer);
  /**
   * @brief Sets the default pipeline configuration.
   * @param config Reference to the PipelineConfiguration struct to set defaults for.
   */
  static void DefaultPipelineConfig(PipelineConfiguration& config);

private:
  /**
   * @brief Reads a file and returns its contents as a vector of characters.
   * @param path The path to the file.
   * @return A vector of characters containing the file contents.
   */
  static std::vector<char> ReadFile(const std::string& path);
  /**
   * @brief Creates the Vulkan graphics pipeline.
   * @param vertPath Path to the vertex shader file.
   * @param fragPath Path to the fragment shader file.
   * @param config Reference to the PipelineConfiguration struct.
   */
  void CreatePipeline(const std::string& vertPath, const std::string& fragPath, const PipelineConfiguration& config);
  /**
   * @brief Creates a Vulkan shader module from the given code.
   * @param code The shader code.
   * @param shaderModule Pointer to the VkShaderModule to create.
   */
  void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

  Devices& m_device;
  VkPipeline m_graphicsPipeline;
  VkShaderModule m_vertShaderModule;
  VkShaderModule m_fragShaderModule;
};

}
