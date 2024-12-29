#include "simple_render_system.hpp"
#include <glm/gtc/constants.hpp>

namespace bloom::render {

/**
* @struct SimplePushConstantData
* @brief Represents the push constant data used in the renderer.
*
* This struct contains transformation and color data that are passed to the shaders
* as push constants.
*/
struct SimplePushConstantData {
  glm::mat4 transform = glm::mat4(1.0f);
  alignas(16) glm::vec3 tint;
};

SimpleRenderSystem::SimpleRenderSystem(Devices* devices) : m_devices(devices) { }
SimpleRenderSystem::~SimpleRenderSystem() {
  vkDestroyPipelineLayout(m_devices->Device(), m_pipelineLayout, nullptr);
}

void SimpleRenderSystem::Begin(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VkDescriptorSetLayout materialSetLayout) {
  CreatePipelineLayout(globalSetLayout, materialSetLayout);
  CreatePipeline(renderPass);
}

void SimpleRenderSystem::CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout, VkDescriptorSetLayout materialSetLayout) {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  std::vector<VkDescriptorSetLayout> descriptorSetLayouts = { globalSetLayout, materialSetLayout };

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = static_cast<unsigned int>(descriptorSetLayouts.size());
  pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  auto result = vkCreatePipelineLayout(m_devices->Device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

  if (result != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to create pipeline layout");
  }
}

void SimpleRenderSystem::CreatePipeline(VkRenderPass renderPass) {
  if (m_pipelineLayout == nullptr) BLOOM_CRITICAL("Pipeline layout is null");
  PipelineConfiguration pipelineConfig{};
  Pipeline::DefaultPipelineConfig(pipelineConfig);
  // Tells what layout to expect to the render buffer
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = m_pipelineLayout;
  m_pipeline = std::make_unique<Pipeline>(*m_devices, "resources/shaders/default.vert.spv", "resources/shaders/default.frag.spv", pipelineConfig);
}

void SimpleRenderSystem::RenderObjects(FrameInfo& frameInfo, ActorMap actors) {
  m_pipeline->Bind(frameInfo.commandBuffer);

  vkCmdBindDescriptorSets(
    frameInfo.commandBuffer,
    VK_PIPELINE_BIND_POINT_GRAPHICS,
    m_pipelineLayout,
    0, 1,
    &frameInfo.globalDescriptorSet,
    0, nullptr
  );

  for (auto &[fst, snd] : actors) {
    auto& actor = snd;
    auto textureDescriptorSet = actor->GetDescriptorSet();

    vkCmdBindDescriptorSets(
      frameInfo.commandBuffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      m_pipelineLayout,
      1, 1,
      &textureDescriptorSet,
      0, nullptr
    );

    actor->transform.rotation.y = glm::mod(actor->transform.rotation.y + 0.001f, glm::two_pi<float>());
    actor->transform.rotation.x = glm::mod(actor->transform.rotation.x + 0.0005f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.tint = actor->tintColor;
    push.transform = actor->transform.Matrix();

    vkCmdPushConstants(
      frameInfo.commandBuffer,
      m_pipelineLayout,
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      0,
      sizeof(SimplePushConstantData),
      &push
    );

    actor->model->Bind(frameInfo.commandBuffer);
    actor->model->Draw(frameInfo.commandBuffer);
  }
}

}