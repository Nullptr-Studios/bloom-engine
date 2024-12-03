#include "engine.hpp"

namespace bloom {

Engine::Engine() { }
Engine::~Engine() {
  vkDestroyPipelineLayout(m_devices->device(), m_pipelineLayout, nullptr);
}

void Engine::Begin() {
  glfwInit();
	Log::Init();

  _window = new Window(800, 600, "Bloom");
  _window->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
  _window->OnInit();

  m_devices = std::make_unique<render::Devices>(*_window);
  m_swapChain = std::make_unique<render::SwapChain>(*m_devices, _window->GetExtent());
  LoadModels();
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
  BLOOM_LOG("{0}", e.ToString());

  if (e.GetEventType() == EventType::WindowClose) {
    _window->CloseWindow();
  }
}

void Engine::CreatePipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  auto result = vkCreatePipelineLayout(m_devices->device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

  if (result != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to create pipeline layout");
  }
}

void Engine::CreatePipeline() { 
  auto pipelineConfig = render::Pipeline::defaultPipelineConfig(m_swapChain->width(), m_swapChain->height());
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

  for (int i = 0; i < m_commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    auto result = vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

    if (result != VK_SUCCESS) {
      BLOOM_CRITICAL("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_swapChain->GetRenderPass();
    renderPassInfo.framebuffer = m_swapChain->GetFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapChain->GetSwapChainExtent();
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    m_pipeline->Bind(m_commandBuffers[i]);
    m_renderer->Bind(m_commandBuffers[i]);
    m_renderer->Draw(m_commandBuffers[i]);
    vkCmdEndRenderPass(m_commandBuffers[i]);
    if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
      BLOOM_CRITICAL("Failed to record command buffer");
    }
  }
}

void Engine::DrawFrame() {
  uint32_t imageIndex;
  auto result = m_swapChain->AcquireNextImage(&imageIndex);
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    BLOOM_ERROR("Failed to acquire next image");
  }

  result = m_swapChain->SubmitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
  if (result != VK_SUCCESS) {
    BLOOM_ERROR("Failed to submit command buffer");
  }
}

void Engine::LoadModels() {
  std::vector<render::Renderer::Vertex> vertices {
  {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
  {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
  {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}}
  };

  m_renderer = std::make_unique<render::Renderer>(m_devices.get(), vertices);
}

} // namespace bloom
