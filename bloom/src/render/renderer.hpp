/**
 * @file renderer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 20/11/2024
 *
 * @brief Main engine class
 */

#pragma once

#include "src/window.hpp"
#include "devices.hpp"
#include "swap_chain.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

struct SimplePushConstantData {
  glm::mat2 transform = glm::mat2(1.0f);
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

class BLOOM_API Renderer {

public:
  Renderer(Window* window, Devices* devices);
  ~Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  VkCommandBuffer BeginFrame();
  void EndFrame();
  void BeginRenderPass(VkCommandBuffer commandBuffer);
  void EndRenderPass(VkCommandBuffer commandBuffer);

  bool GetFrameStarted() const { return m_frameStarted; }
  VkCommandBuffer GetCurrentCommandBuffer() const {
    if (!m_frameStarted) { BLOOM_WARN("Cannot get CommandBuffer when frame not in progress"); }
    return m_commandBuffers[m_currentFrameIndex];
  }
  VkRenderPass GetRenderPass() const { return m_swapChain->GetRenderPass(); }

  int GetFrameIndex() const {
    if (!m_frameStarted) {
      BLOOM_WARN("Cannot get frame index when frame not in progress");
      return -1;
    }
    return m_currentFrameIndex;
  }

protected:
  void CreateCommandBuffers();
  void FreeCommandBuffers();
  void RecreateSwapChain();

  Window* m_window = nullptr;
  Devices* m_devices = nullptr;
  std::unique_ptr<SwapChain> m_swapChain = nullptr;
  std::vector<VkCommandBuffer> m_commandBuffers;

  unsigned int m_currentImageIndex = 0;
  int m_currentFrameIndex;
  bool m_frameStarted = false;
};

}
