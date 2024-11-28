#include "engine.hpp"

namespace bloom {

Engine::Engine() { }
Engine::~Engine() {
  vkDestroyPipelineLayout(_devices->device(), _pipelineLayout, nullptr);
}

void Engine::Begin() {
  glfwInit();
	Log::Init();

  _window = new Window(800, 600, "Bloom");
  _window->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
  _window->OnInit();

  _devices = std::make_unique<render::Devices>(*_window);
  _swapChain = std::make_unique<render::SwapChain>(*_devices, _window->GetExtent());
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
  vkDeviceWaitIdle(_devices->device());
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
  auto result = vkCreatePipelineLayout(_devices->device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout);

  if (result != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to create pipeline layout");
  }
}

void Engine::CreatePipeline() { 
  auto pipelineConfig = render::Pipeline::defaultPipelineConfig(_swapChain->width(), _swapChain->height());
  // Tells what layout to expect to the render buffer
  pipelineConfig.renderPass = _swapChain->GetRenderPass();
  pipelineConfig.pipelineLayout = _pipelineLayout;
  _pipeline = std::make_unique<render::Pipeline>(*_devices, "resources/shaders/default.vert.spv", "resources/shaders/default.frag.spv", pipelineConfig);
}

void Engine::CreateCommandBuffers() {
  _commandBuffers.resize(_swapChain->ImageCount());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = _devices->getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());
  auto allocResult = vkAllocateCommandBuffers(_devices->device(), &allocInfo, _commandBuffers.data());

  if (allocResult != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to allocate command buffers");
  }

  for (int i = 0; i < _commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    auto result = vkBeginCommandBuffer(_commandBuffers[i], &beginInfo);

    if (result != VK_SUCCESS) {
      BLOOM_CRITICAL("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = _swapChain->GetRenderPass();
    renderPassInfo.framebuffer = _swapChain->GetFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = _swapChain->GetSwapChainExtent();
    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    _pipeline->Bind(_commandBuffers[i]);
    vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);
    vkCmdEndRenderPass(_commandBuffers[i]);
    if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
      BLOOM_CRITICAL("Failed to record command buffer");
    }
  }
}

void Engine::DrawFrame() {
  uint32_t imageIndex;
  auto result = _swapChain->AcquireNextImage(&imageIndex);
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    BLOOM_ERROR("Failed to acquire next image");
  }

  result = _swapChain->SubmitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
  if (result != VK_SUCCESS) {
    BLOOM_ERROR("Failed to submit command buffer");
  }
}

} // namespace bloom
