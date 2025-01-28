#include "renderer.hpp"
#include "src/window.hpp"
#include "devices.hpp"
#include "swap_chain.hpp"

namespace bloom::render {

Renderer* Renderer::m_instance = nullptr;

Renderer::Renderer(Window* window, Devices* devices) : m_window(window), m_devices(devices) {
  m_instance = this;
  RecreateSwapChain();
  CreateCommandBuffers();
}
Renderer::~Renderer() {
  m_instance = nullptr;
  FreeCommandBuffers();
}

VkCommandBuffer Renderer::BeginFrame() {
  if (m_frameStarted) {
    BLOOM_WARN("Can't call BeginFrame while already in progress");
    return nullptr;
  }

  auto result = m_swapChain->AcquireNextImage(&m_currentImageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    RecreateSwapChain();
    return nullptr;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    BLOOM_ERROR("Failed to acquire next image");
  }

  m_frameStarted = true;

  auto commandBuffer = GetCurrentCommandBuffer();
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  result = vkBeginCommandBuffer(commandBuffer, &beginInfo);

  if (result != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to begin recording command buffer");
  }

  return commandBuffer;
}

void Renderer::EndFrame() {
  if (!m_frameStarted) {
    BLOOM_WARN("Can't call EndFrame without frame started");
    return;
  }
  auto commandBuffer = GetCurrentCommandBuffer();
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to record command buffer");
  }

  auto result = m_swapChain->SubmitCommandBuffers(&commandBuffer, &m_currentImageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_window->GetWindowResized()) {
    m_window->ResetWindowResized();
    RecreateSwapChain();
  } else if (result != VK_SUCCESS) {
    BLOOM_ERROR("Failed to submit command buffer");
  }

  m_frameStarted = false;
  // TODO: This should be a char
  m_currentFrameIndex = (m_currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::BeginRenderPass(VkCommandBuffer commandBuffer) const {
  if (!m_frameStarted) {
    BLOOM_WARN("Can't call BeginRenderPass if frame is not in progress");
    return;
  }
  if (commandBuffer != GetCurrentCommandBuffer()) {
    BLOOM_WARN("Can't begin render pass on command buffer from a different frame");
    return;
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_swapChain->GetRenderPass();
  renderPassInfo.framebuffer = m_swapChain->GetFrameBuffer(m_currentImageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = m_swapChain->GetSwapChainExtent();
  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(m_swapChain->GetSwapChainExtent().width);
  viewport.height = static_cast<float>(m_swapChain->GetSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, m_swapChain->GetSwapChainExtent()};
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::EndRenderPass(VkCommandBuffer commandBuffer) const {
  if (!m_frameStarted) {
    BLOOM_WARN("Can't call EndRenderPass if frame is not in progress");
    return;
  }
  if (commandBuffer != GetCurrentCommandBuffer()) {
    BLOOM_WARN("Can't end render pass on command buffer from a different frame");
    return;
  }

  vkCmdEndRenderPass(commandBuffer);
}

void Renderer::CreateCommandBuffers() {
  m_commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = m_devices->GetCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
  auto allocResult = vkAllocateCommandBuffers(m_devices->Device(), &allocInfo, m_commandBuffers.data());

  if (allocResult != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to allocate command buffers");
  }
}

void Renderer::FreeCommandBuffers() {
  vkFreeCommandBuffers(
    m_devices->Device(),
    m_devices->GetCommandPool(),
    static_cast<unsigned int>(m_commandBuffers.size()),
    m_commandBuffers.data());
  m_commandBuffers.clear();
}

void Renderer::RecreateSwapChain() {
  auto extent = m_window->GetExtent();
  // Makes the program wait while minimized
  while (extent.width == 0 || extent.height == 0) {
    extent = m_window->GetExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(m_devices->Device());

  if (m_swapChain == nullptr)
    m_swapChain = std::make_unique<render::SwapChain>(*m_devices, extent);
  else {
    std::shared_ptr<SwapChain> oldSwapChain = std::move(m_swapChain);
    m_swapChain = std::make_unique<render::SwapChain>(*m_devices, extent, oldSwapChain);

    if (!oldSwapChain->CompareSwapFormats(*m_swapChain.get())) {
      BLOOM_WARN("Swap chain image format or depth format changed, recreating command buffers");
      FreeCommandBuffers();
      CreateCommandBuffers();
    }
  }
  // CreatePipeline();
}

}
