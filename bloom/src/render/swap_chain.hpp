/**
 * @file swap_chain.hpp
 * @author Brendan Galea
 * @date 12/12/2024
 *
 * @brief Swap chain implementation file
 */

#pragma once

#include "devices.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

/**
 * @class SwapChain
 * @brief Manages the Vulkan swap chain and related resources.
 *
 * The SwapChain class handles the creation and management of the Vulkan swap chain, including
 * image views, framebuffers, depth resources, and synchronization objects. It also provides
 * methods for acquiring the next image and submitting command buffers.
 */
class SwapChain {
public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2; ///< Maximum number of chains

  SwapChain(Devices &deviceRef, VkExtent2D windowExtent);
  SwapChain(Devices &deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
  ~SwapChain();

  // Copy constructors
  SwapChain(const SwapChain &) = delete;
  SwapChain& operator=(const SwapChain &) = delete;

  // region Getters
  /**
   * @brief Gets the framebuffer at the specified index.
   * @param index The index of the framebuffer.
   * @return The Vulkan framebuffer.
   */
  VkFramebuffer GetFrameBuffer(int index) const { return m_swapChainFramebuffers[index]; }
  /**
   * @brief Gets the render pass.
   * @return The Vulkan render pass.
   */
  VkRenderPass GetRenderPass() const { return m_renderPass; }
  /**
   * @brief Gets the image view at the specified index.
   * @param index The index of the image view.
   * @return The Vulkan image view.
   */
  VkImageView GetImageView(int index) const { return m_swapChainImageViews[index]; }
  /**
   * @brief Gets the number of images in the swap chain.
   * @return The number of images.
   */
  size_t ImageCount() const { return m_swapChainImages.size(); }
  /**
   * @brief Gets the format of the swap chain images.
   * @return The Vulkan format of the swap chain images.
   */
  VkFormat GetSwapChainImageFormat() const { return m_swapChainImageFormat; }
  /**
   * @brief Gets the extent of the swap chain.
   * @return The Vulkan extent of the swap chain.
   */
  VkExtent2D GetSwapChainExtent() const { return m_swapChainExtent; }
  /**
   * @brief Gets the width of the swap chain extent.
   * @return The width of the swap chain extent.
   */
  uint32_t Width() const { return m_swapChainExtent.width; }
  /**
   * @brief Gets the height of the swap chain extent.
   * @return The height of the swap chain extent.
   */
  uint32_t Height() const { return m_swapChainExtent.height; }
  /**
   * @brief Gets the aspect ratio of the swap chain extent.
   * @return The aspect ratio.
   */
  float ExtentAspectRatio() const {
    return static_cast<float>(m_swapChainExtent.width) / static_cast<float>(m_swapChainExtent.height);
  }

  // region Functions
  /**
   * @brief Finds the depth format for the swap chain.
   * @return The Vulkan format for depth.
   */
  VkFormat FindDepthFormat();
  /**
   * @brief Acquires the next image in the swap chain.
   * @param imageIndex Pointer to the index of the acquired image.
   * @return The Vulkan result of the acquisition.
   */
  VkResult AcquireNextImage(uint32_t *imageIndex);
  /**
   * @brief Submits the command buffers for rendering.
   * @param buffers Pointer to the command buffers.
   * @param imageIndex Pointer to the index of the image to submit.
   * @return The Vulkan result of the submission.
   */
  VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);
  /**
   * @brief Compares the formats of two swap chains.
   * @param swapChain Reference to the other SwapChain instance.
   * @return True if the formats are the same, false otherwise.
   */
  bool CompareSwapFormats(const SwapChain& swapChain) const {
    return swapChain.m_swapChainDepthFormat == m_swapChainDepthFormat &&
           swapChain.m_swapChainImageFormat == m_swapChainImageFormat;
  }

private:
  /**
   * @brief Initializes the swap chain.
   */
  void Init();
  /**
   * @brief Creates the swap chain.
   */
  void CreateSwapChain();
  /**
   * @brief Creates the image views for the swap chain images.
   */
  void CreateImageViews();
  /**
   * @brief Creates the depth resources for the swap chain.
   */
  void CreateDepthResources();
  /**
   * @brief Creates the render pass.
   */
  void CreateRenderPass();
  /**
   * @brief Creates the framebuffers for the swap chain.
   */
  void CreateFramebuffers();
  /**
   * @brief Creates the synchronization objects for the swap chain.
   */
  void CreateSyncObjects();
  /**
   * @brief Chooses the swap surface format.
   * @param availableFormats The available surface formats.
   * @return The chosen surface format.
   */
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  /**
   * @brief Chooses the swap present mode.
   * @param availablePresentModes The available present modes.
   * @return The chosen present mode.
   */
  VkPresentModeKHR ChooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  /**
   * @brief Chooses the swap extent.
   * @param capabilities The surface capabilities.
   * @return The chosen extent.
   */
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

}
