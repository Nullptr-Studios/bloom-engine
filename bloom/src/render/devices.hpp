/**
 * @file devices.hpp
 * @author Brendan Galea
 * @date 27/11/2024
 *
 * @brief Vulkan Validation Layers file
 */

#pragma once

#include "src/window.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

/**
 * @struct SwapChainSupportDetails
 * @brief Swap chain support details.
 *
 * This structure holds details about the swap chain support for a given physical
 * device.  This includes the capabilities, supported surface formats, and
 * available present modes.
 */
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes; ///< Supported present modes. Defines the available presentation methods (e.g., FIFO, mailbox).
};


/**
 * @struct QueueFamilyIndices
 * @brief Queue family indices.
 *
 * Contains the indices of the required queue families for graphics and
 * presentation operations. These indices are used to retrieve the corresponding
 * queues from the logical device.
 */
struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  bool graphicsFamilyHasValue = false;
  bool presentFamilyHasValue = false;
  bool IsComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

/**
 * @class Devices
 * @brief Manages Vulkan devices and related resources.
 *
 * This class encapsulates the initialization and management of Vulkan devices,
 * instances, surfaces, and other related resources. It provides methods for
 * creating and configuring these resources, as well as helper functions for
 * buffer and image operations. It serves as the central point of interaction
 * with the Vulkan API.
 */
class BLOOM_API Devices {
public:
// Validation layers check
#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

  explicit Devices(Window &window);
  virtual ~Devices();

  // Copy constructors
  Devices(const Devices &) = delete;
  Devices& operator=(const Devices &) = delete;
  Devices(Devices &&) = delete;
  Devices& operator=(Devices &&) = delete;

  // region Getters
  /**
   * @brief Gets the command pool.
   * @return The command pool handle. Used for allocating command buffers.
   */
  VkCommandPool GetCommandPool() const { return m_commandPool; }
  /**
   * @brief Gets the logical device.
   * @return The logical device handle. The interface to the physical device.
   */
  VkDevice Device() const { return m_device; }
  /**
   * @brief Gets the Vulkan surface.
   * @return The surface handle. Represents the connection between the window and Vulkan.
   */
  VkSurfaceKHR Surface() const { return m_surface; }
  /**
   * @brief Gets the graphics queue.
   * @return The graphics queue handle. Used for submitting graphics commands.
   */
  VkQueue GraphicsQueue() const { return m_graphicsQueue; }
  /**
   * @brief Gets the present queue.
   * @return The present queue handle. Used for presenting images to the surface.
   */
  VkQueue PresentQueue() const { return m_presentQueue; }

  /**
   * @brief Gets swap chain support details.
   * @return The swap chain support details for the physical device. Provides information about supported capabilities, formats, and present modes.
   */
  SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(m_physicalDevice); }
  /**
   * @brief Finds a suitable memory type.
   * @param typeFilter Bitmask of memory types to filter.
   * @param properties Desired memory properties.
   * @return Index of a suitable memory type that matches the filter and properties.
   */
  uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  /**
   * @brief Finds suitable queue families.
   * @return QueueFamilyIndices containing the found graphics and present queue family indices.
   */
  QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_physicalDevice); }
  /**
   * @brief Finds a supported format.
   * @param candidates List of candidate formats.
   * @param tiling Image tiling mode.
   * @param features Desired format features.
   * @return The first supported format found, or VK_FORMAT_UNDEFINED if none are supported.
   */
  VkFormat FindSupportedFormat(
      const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  /**
   * @brief Gets the physical device.
   * @return The physical device handle. Represents the GPU.
   */
  static Devices* GetInstance() {
    if (!m_instance) {
      BLOOM_ERROR("Tried to access Devices but it has not been created yet");
      return nullptr;
    }
    return m_instance;
  }

  // region Helper Functions
  /**
   * @brief Creates a Vulkan buffer.
   * @param size The size of the buffer in bytes.
   * @param usage The intended usage of the buffer.
   * @param properties The desired memory properties for the buffer.
   * @param buffer Output parameter for the created buffer handle.
   * @param bufferMemory Output parameter for the allocated buffer memory handle.  The memory associated with the buffer.
   */
  void CreateBuffer(
      VkDeviceSize size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties,
      VkBuffer &buffer,
      VkDeviceMemory &bufferMemory);
  /**
   * @brief Begins single-time commands.
   * @return A command buffer for recording single-time use commands.  This is typically used for operations like transferring data between buffers and images.
   */
  VkCommandBuffer BeginSingleTimeCommands();
  /**
   * @brief Ends and submits single-time commands.
   * @param commandBuffer The command buffer to end and submit. Executes the recorded commands.
   */
  void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
  /**
   * @brief Copies data between buffers.
   * @param srcBuffer The source buffer.
   * @param dstBuffer The destination buffer.
   * @param size The size of the data to copy in bytes.
   */
  void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  /**
   * @brief Copies data from buffer to image.
   * @param buffer The source buffer.
   * @param image The destination image.
   * @param width The width of the image.
   * @param height The height of the image.
   * @param layerCount The number of layers in the image.
   */
  void CopyBufferToImage(
      VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
  /**
   * @brief Creates a Vulkan image.
   * @param imageInfo The image create info structure.  Describes the parameters of the image to be created.
   * @param properties The desired memory properties for the image.
   * @param image Output parameter for the created image handle.
   * @param imageMemory Output parameter for the allocated image memory handle. The memory associated with the image.
   */
  void CreateImageWithInfo(
      const VkImageCreateInfo &imageInfo,
      VkMemoryPropertyFlags properties,
      VkImage &image,
      VkDeviceMemory &imageMemory);

  VkPhysicalDeviceProperties properties;

private:
  void CreateInstance();        ///< Creates a Vulkan instance
  void SetupDebugMessenger();   ///< Sets up the debug messenger for validation layers
  void CreateSurface();         ///< Creates a window surface
  void PickPhysicalDevice();    ///< Selects a suitable physical device
  void CreateLogicalDevice();   ///< Creates a logical device
  void CreateCommandPool();     ///< Creates a command pool

  // helper functions
  bool IsDeviceSuitable(VkPhysicalDevice device);
  std::vector<const char *> GetRequiredExtensions();
  bool CheckValidationLayerSupport();
  QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
  void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void HasGflwRequiredInstanceExtensions();
  bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

  static Devices* m_instance;
  VkInstance m_vkInstance;
  VkDebugUtilsMessengerEXT m_debugMessenger;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  Window &m_window;
  VkCommandPool m_commandPool;

  VkDevice m_device;
  VkSurfaceKHR m_surface;
  VkQueue m_graphicsQueue;
  VkQueue m_presentQueue;

  const std::vector<const char *> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char *> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

}
