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

/**
 * @namespace bloom::render
 * @brief Contains classes and functions related to rendering in the bloom engine.
 *
 * The @c bloom::render namespace encapsulates all the components and utilities required for rendering
 * graphics using Vulkan. This includes managing devices, pipelines, models, shaders, and other
 * rendering-related resources.
 *
 * @details
 * The @c bloom::render namespace is designed to provide a comprehensive set of tools for rendering
 * 3D graphics. It includes classes for managing Vulkan devices, creating and managing graphics
 * pipelines, handling models and their vertex/index buffers, and more. The namespace is structured
 * to facilitate the creation, management, and rendering of 3D objects in a Vulkan-based rendering
 * engine.
 *
 * Key Classes:
 * - @b Devices: Manages Vulkan devices and related resources.
 * - @b Pipeline: Manages Vulkan graphics pipelines.
 * - @b Model: Represents a 3D model and manages its vertex and index buffers.
 * - @b Renderer: Handles the rendering process, including command buffer management and swap chain recreation.
 * - @b SwapChain: Manages the Vulkan swap chain for rendering.
 */
namespace bloom::render {

/**
 * @class Renderer
 * @brief Manages the rendering process in the bloom engine.
 *
 * The Renderer class handles the rendering process, including command buffer management,
 * swap chain recreation, and rendering passes.
 */
class BLOOM_API Renderer {

public:
  Renderer(Window* window, Devices* devices);
  ~Renderer();

  // Copy constructors
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  /**
   * @brief Begins a new frame and returns the command buffer for the frame.
   * @return The Vulkan command buffer for the current frame.
   */
  VkCommandBuffer BeginFrame();
  /**
   * @brief Ends the current frame.
   */
  void EndFrame();
  /**
   * @brief Begins the render pass for the current frame.
   * @param commandBuffer The Vulkan command buffer to begin the render pass with.
   */
  void BeginRenderPass(VkCommandBuffer commandBuffer) const;
  /**
   * @brief Ends the render pass for the current frame.
   * @param commandBuffer The Vulkan command buffer to end the render pass with.
   */
  void EndRenderPass(VkCommandBuffer commandBuffer) const;

  // region Getters
  /**
   * @brief Checks if a frame has started.
   * @return True if a frame has started, false otherwise.
   */
  bool GetFrameStarted() const { return m_frameStarted; }
  /**
   * @brief Gets the command buffer for the current frame.
   * @return The Vulkan command buffer for the current frame.
   */
  VkCommandBuffer GetCurrentCommandBuffer() const {
    if (!m_frameStarted) BLOOM_WARN("Cannot get CommandBuffer when frame not in progress");
    return m_commandBuffers[m_currentFrameIndex];
  }
  /**
   * @brief Gets the render pass.
   * @return The Vulkan render pass.
   */
  VkRenderPass GetRenderPass() const { return m_swapChain->GetRenderPass(); }
  /**
   * @brief Gets the aspect ratio of the swap chain extent.
   * @return The aspect ratio of the swap chain extent.
   */
  float GetAspectRatio() const { return m_swapChain->ExtentAspectRatio(); }
  /**
   * @brief Gets the index of the current frame.
   * @return The index of the current frame, or -1 if no frame is in progress.
   */
  int GetFrameIndex() const {
    if (!m_frameStarted) {
      BLOOM_WARN("Cannot get frame index when frame not in progress");
      return -1;
    }
    return m_currentFrameIndex;
  }
  /**
   * @brief Gets the renderer instance
   * @return Pointer to the renderer instance
   */
  static Renderer* GetInstance() {
    if (m_instance == nullptr) {
      BLOOM_ERROR("Trying to get renderer but it has not been created yet");
      return nullptr;
    }
    return m_instance;
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
  int m_currentFrameIndex = 0;
  bool m_frameStarted = false;

private:
  static Renderer* m_instance;
};

}
