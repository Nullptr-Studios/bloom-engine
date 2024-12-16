/**
 * \file swap_chain.hpp
 * \author Brendan Galea
 * \date 12/12/2024
 *
 * \brief Swap chain implementation file
 */

#pragma once

#include "devices.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

class SwapChain {
public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  SwapChain(Devices &deviceRef, VkExtent2D windowExtent);
  SwapChain(Devices &deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
  ~SwapChain();

  SwapChain(const SwapChain &) = delete;
  SwapChain& operator=(const SwapChain &) = delete;

  VkFramebuffer GetFrameBuffer(int index) { return m_swapChainFramebuffers[index]; }
  VkRenderPass GetRenderPass() { return m_renderPass; }
  VkImageView GetImageView(int index) { return m_swapChainImageViews[index]; }
  size_t ImageCount() { return m_swapChainImages.size(); }
  VkFormat GetSwapChainImageFormat() { return m_swapChainImageFormat; }
  VkExtent2D GetSwapChainExtent() { return m_swapChainExtent; }
  uint32_t width() { return m_swapChainExtent.width; }
  uint32_t height() { return m_swapChainExtent.height; }

  float ExtentAspectRatio() {
    return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height);
  }
  VkFormat FindDepthFormat();

  VkResult AcquireNextImage(uint32_t *imageIndex);
  VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

  bool compareSwapFormats(const SwapChain& swapChain) const {
    return swapChain.m_swapChainDepthFormat == m_swapChainDepthFormat &&
           swapChain.m_swapChainImageFormat == m_swapChainImageFormat;
  }

private:
  void Init();
  void CreateSwapChain();
  void CreateImageViews();
  void CreateDepthResources();
  void CreateRenderPass();
  void CreateFramebuffers();
  void CreateSyncObjects();

  // Helper functions
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR ChooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VkFormat m_swapChainImageFormat;
  VkFormat m_swapChainDepthFormat;
  VkExtent2D m_swapChainExtent;

  std::vector<VkFramebuffer> m_swapChainFramebuffers;
  VkRenderPass m_renderPass;

  std::vector<VkImage> m_depthImages;
  std::vector<VkDeviceMemory> m_depthImageMemories;
  std::vector<VkImageView> m_depthImageViews;
  std::vector<VkImage> m_swapChainImages;
  std::vector<VkImageView> m_swapChainImageViews;

  Devices &m_device;
  VkExtent2D m_windowExtent;

  VkSwapchainKHR m_swapChain;
  std::shared_ptr<SwapChain> m_oldSwapChain;

  std::vector<VkSemaphore> m_imageAvailableSemaphores;
  std::vector<VkSemaphore> m_renderFinishedSemaphores;
  std::vector<VkFence> m_inFlightFences;
  std::vector<VkFence> m_imagesInFlight;
  size_t m_currentFrame = 0;
};

}  // namespace lve
