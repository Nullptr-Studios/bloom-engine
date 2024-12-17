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

void SimpleRenderSystem::Begin(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, VkDescriptorSetLayout materialSetLayout) {
  CreateDescriptorPool();
  CreateDescriptorSets();
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

void SimpleRenderSystem::RenderObjects(render::FrameInfo& frameInfo, ActorMap actors) {
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
    push.color = actor->tintColor;
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

void SimpleRenderSystem::CreateDescriptorPool() {
  // std::vector<VkDescriptorPoolSize> poolSizes{};
  // poolSizes.resize(1);
  // poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  // poolSizes[0].descriptorCount = static_cast<unsigned int>(m_poolSizes.imageSampler);
  //
  // m_globalPool = std::make_unique<render::DescriptorPool>(m_devices, static_cast<unsigned int>(poolSizes.size()));
}

void SimpleRenderSystem::CreateDescriptorSets() {
  // m_globalDescriptorSets.resize(1);
  // for (size_t i = 0; i < 1; i++) {
  //   m_globalDescriptorSets[i] = AllocateDescriptorSet(m_textureLayout->getDescriptorSetLayout());
  // }
}

// VkDescriptorSet SimpleRenderSystem::AllocateDescriptorSet(VkDescriptorSetLayout layout) {
//   VkDescriptorSetAllocateInfo allocInfo{};
//   allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//   allocInfo.descriptorPool = m_globalPool->getDescriptorPool();
//   allocInfo.descriptorSetCount = 1;
//   allocInfo.pSetLayouts = &layout;
//   VkDescriptorSet descriptorSet;
//   auto result = vkAllocateDescriptorSets(m_devices->device(), &allocInfo, &descriptorSet);
//   if (result != VK_SUCCESS) {
//     BLOOM_CRITICAL("Failed to allocate descriptor set");
//   }
//   return descriptorSet;
// }

}