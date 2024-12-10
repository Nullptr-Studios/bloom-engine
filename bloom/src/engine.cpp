#include "engine.hpp"

#include "glm/gtc/constants.hpp"

namespace bloom {

Engine::Engine() { }
Engine::~Engine() {
  vkDestroyPipelineLayout(m_devices->device(), m_pipelineLayout, nullptr);
}

void Engine::Begin() {
  glfwInit();
	Log::Init();

  _window = new Window(800, 800, "Bloom");
  _window->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
  _window->OnInit();

  factory = std::make_unique<Factory>();

  m_devices = std::make_unique<render::Devices>(*_window);
  m_swapChain = std::make_unique<render::SwapChain>(*m_devices, _window->GetExtent());
  LoadObjects();
  CreatePipelineLayout();
  CreatePipeline();
  CreateCommandBuffers();
}

void Engine::Tick() {
  _window->OnTick();
}

void Engine::Render() {
  DrawFrame();
}

void Engine::End() {
  vkDeviceWaitIdle(m_devices->device());
  delete _window;
}

void Engine::OnEvent(Event &e) {
  BLOOM_INFO("{0}", e.ToString());

  if (e.GetEventType() == EventType::WindowClose) {
    _window->CloseWindow();
  }
}

void Engine::CreatePipelineLayout() {
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

void Engine::CreatePipeline() {
  if (m_swapChain == nullptr) BLOOM_CRITICAL("Swap chain is null");
  if (m_pipelineLayout == nullptr) BLOOM_CRITICAL("Pipeline layout is null");
  render::PipelineConfiguration pipelineConfig{};
  render::Pipeline::defaultPipelineConfig(pipelineConfig);
  // Tells what layout to expect to the render buffer
  pipelineConfig.renderPass = m_swapChain->GetRenderPass();
  pipelineConfig.pipelineLayout = m_pipelineLayout;
  m_pipeline = std::make_unique<render::Pipeline>(*m_devices, "resources/shaders/default.vert.spv", "resources/shaders/default.frag.spv", pipelineConfig);
}

void Engine::CreateCommandBuffers() {
  m_commandBuffers.resize(m_swapChain->ImageCount());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = m_devices->getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
  auto allocResult = vkAllocateCommandBuffers(m_devices->device(), &allocInfo, m_commandBuffers.data());

  if (allocResult != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to allocate command buffers");
  }
}

void Engine::FreeCommandBuffers() {
  vkFreeCommandBuffers(
    m_devices->device(),
    m_devices->getCommandPool(),
    static_cast<unsigned int>(m_commandBuffers.size()),
    m_commandBuffers.data());
  m_commandBuffers.clear();
}

void Engine::DrawFrame() {
  uint32_t imageIndex;
  auto result = m_swapChain->AcquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    RecreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    BLOOM_ERROR("Failed to acquire next image");
  }

  RecordCommandBuffer(imageIndex);
  result = m_swapChain->SubmitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window->GetWindowResized()) {
    _window->ResetWindowResized();
    RecreateSwapChain();
    return;
  }
  if (result != VK_SUCCESS) {
    BLOOM_ERROR("Failed to submit command buffer");
  }
}

void Engine::LoadObjects() {
  std::vector<render::Renderer::Vertex> vertices {
  {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
  {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
  {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}}
};

  auto m_model = std::make_shared<render::Renderer>(m_devices.get(), vertices);

  auto triangle = factory->CreateObject<Object>();
  triangle.model = m_model;
  triangle.color = {0.8f, 0.1f, 0.8f};
  triangle.transform.position.x = 0.0f;
  triangle.transform.scale = {1.0f, 1.0f};
  triangle.transform.rotation = 0.25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

void Engine::RenderObjects(VkCommandBuffer commandBuffer) {
  m_pipeline->Bind(commandBuffer);

  for (auto& obj : gameObjects) {
    obj.transform.rotation = glm::mod(obj.transform.rotation + 0.0001f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.offset = obj.transform.position;
    push.color = obj.color;
    push.transform = obj.transform.mat2();

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

void Engine::RecreateSwapChain() {
  auto extent = _window->GetExtent();
  // Makes the program wait while minimized
  while (extent.width == 0 || extent.height == 0) {
    extent = _window->GetExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(m_devices->device());

  if (m_swapChain == nullptr) {
    m_swapChain = std::make_unique<render::SwapChain>(*m_devices, extent);
  } else {
    m_swapChain = std::make_unique<render::SwapChain>(*m_devices, extent, std::move(m_swapChain));
    if (m_swapChain->ImageCount() != m_commandBuffers.size()) {
      FreeCommandBuffers();
      CreateCommandBuffers();
    }
  }
  CreatePipeline();
}

void Engine::RecordCommandBuffer(int index) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  auto result = vkBeginCommandBuffer(m_commandBuffers[index], &beginInfo);

  if (result != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to begin recording command buffer");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_swapChain->GetRenderPass();
  renderPassInfo.framebuffer = m_swapChain->GetFrameBuffer(index);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = m_swapChain->GetSwapChainExtent();
  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(m_commandBuffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(m_swapChain->GetSwapChainExtent().width);
  viewport.height = static_cast<float>(m_swapChain->GetSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, m_swapChain->GetSwapChainExtent()};
  vkCmdSetViewport(m_commandBuffers[index], 0, 1, &viewport);
  vkCmdSetScissor(m_commandBuffers[index], 0, 1, &scissor);

  RenderObjects(m_commandBuffers[index]);

  vkCmdEndRenderPass(m_commandBuffers[index]);
  if (vkEndCommandBuffer(m_commandBuffers[index]) != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to record command buffer");
  }
}

} // namespace bloom
