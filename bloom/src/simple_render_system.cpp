#include "simple_render_system.hpp"
#include "glm/gtc/constants.hpp"

namespace bloom {

struct SimplePushConstantData {
  glm::mat4 transform = glm::mat4(1.0f);
  alignas(16) glm::vec3 color;
};

SimpleRenderSystem::SimpleRenderSystem(render::Devices* devices) : m_devices(devices) { }
SimpleRenderSystem::~SimpleRenderSystem() {
  vkDestroyPipelineLayout(m_devices->device(), m_pipelineLayout, nullptr);
}

void SimpleRenderSystem::Begin(VkRenderPass renderPass) {
  CreatePipelineLayout();
  CreatePipeline(renderPass);
}

void SimpleRenderSystem::CreatePipelineLayout() {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  auto result = vkCreatePipelineLayout(m_devices->device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

  if (result != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to create pipeline layout");
  }
}

void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass) {
  if (m_pipelineLayout == nullptr) BLOOM_CRITICAL("Pipeline layout is null");
  render::PipelineConfiguration pipelineConfig{};
  render::Pipeline::defaultPipelineConfig(pipelineConfig);
  // Tells what layout to expect to the render buffer
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = m_pipelineLayout;
  m_pipeline = std::make_unique<render::Pipeline>(*m_devices, "resources/shaders/default.vert.spv", "resources/shaders/default.frag.spv", pipelineConfig);
}

void SimpleRenderSystem::RenderObjects(VkCommandBuffer commandBuffer, std::vector<Object>& objects) {
  m_pipeline->Bind(commandBuffer);

  for (auto& obj : objects) {
    obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.0001f, glm::two_pi<float>());
    obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.00005f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.color = obj.color;
    push.transform = obj.transform.mat4();

    vkCmdPushConstants(
      commandBuffer,
      m_pipelineLayout,
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      0,
      sizeof(SimplePushConstantData),
      &push
    );

    obj.model->Bind(commandBuffer);
    obj.model->Draw(commandBuffer);
  }
}

}