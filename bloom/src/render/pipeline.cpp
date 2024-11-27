#include "pipeline.hpp"
#include <fstream>

namespace bloom::render {

Pipeline::Pipeline(Devices &device, const std::string &vertPath, const std::string &fragPath,
    const PipelineConfiguration &config) : _device(device) {
  CreatePipeline(vertPath, fragPath, config);
}

PipelineConfiguration Pipeline::defaultPipelineConfig(uint32_t width, uint32_t height) {
  PipelineConfiguration config{};

  return config;
}

std::vector<char> Pipeline::ReadFile(const std::string &path) {
	std::ifstream file{path, std::ios::ate | std::ios::binary};
  
  // Check if we can open the file
  if (!file.is_open()) {
    BLOOM_ERROR("Failed to open file {0}", path);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.close();
  return buffer;
}

void Pipeline::CreatePipeline(const std::string &vertPath, const std::string &fragPath, const PipelineConfiguration &config) {
  auto vertShader = ReadFile(vertPath);
  auto fragShader = ReadFile(fragPath);

  BLOOM_LOG("Vertex Shader Size: {0}", vertShader.size());
  BLOOM_LOG("Fragment Shader Size: {0}", fragShader.size());
}

void Pipeline::CreateShaderModule(const std::vector<char> &code, VkShaderModule*shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(_device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
    BLOOM_ERROR("Failed to create shader module");
  }
}

}
