#pragma once

#include "devices.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

class SwapChain {
public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  SwapChain(Devices &deviceRef, VkExtent2D windowExtent);
  ~SwapChain();

  SwapChain(const SwapChain &) = delete;
  void operator=(const SwapChain &) = delete;

  VkFramebuffer GetFrameBuffer(int index) { return SwapChainFramebuffers_[index]; }
  VkRenderPass GetRenderPass() { return renderPass_; }
  VkImageView GetImageView(int index) { return swapChainImageViews_[index]; }
  size_t ImageCount() { return swapChainImages_.size(); }
  VkFormat GetSwapChainImageFormat() { return swapChainImageFormat_; }
  VkExtent2D GetSwapChainExtent() { return swapChainExtent_; }
  uint32_t width() { return swapChainExtent_.width; }
  uint32_t height() { return swapChainExtent_.height; }

  float ExtentAspectRatio() {
    return static_cast<float>(swapChainExtent_.width) / static_cast<float>(swapChainExtent_.height);
  }
  VkFormat FindDepthFormat();

  VkResult AcquireNextImage(uint32_t *imageIndex);
  VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

private:
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

  VkFormat swapChainImageFormat_;
  VkExtent2D swapChainExtent_;

  std::vector<VkFramebuffer> SwapChainFramebuffers_;
  VkRenderPass renderPass_;

  std::vector<VkImage> DepthImages_;
  std::vector<VkDeviceMemory> depthImageMemorys_;
  std::vector<VkImageView> depthImageViews_;
  std::vector<VkImage> swapChainImages_;
  std::vector<VkImageView> swapChainImageViews_;

  Devices &_device;
  VkExtent2D windowExtent_;

  VkSwapchainKHR swapChain_;

  std::vector<VkSemaphore> imageAvailableSemaphores_;
  std::vector<VkSemaphore> renderFinishedSemaphores_;
  std::vector<VkFence> inFlightFences_;
  std::vector<VkFence> imagesInFlight_;
  size_t currentFrame_ = 0;
};

}  // namespace lve
