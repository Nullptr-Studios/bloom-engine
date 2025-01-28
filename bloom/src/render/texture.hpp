/**
 * @file texture.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 11/12/2024
 *
 * @brief Defines the Texture class for managing Vulkan textures.
 *
 * This file contains the declaration of the Texture class, which is responsible for loading,
 * managing, and providing access to Vulkan textures in the bloom rendering engine.
 */

#pragma once

namespace bloom::render {
class Devices;

/**
 * @class Texture
 * @brief Manages Vulkan textures.
 *
 * The Texture class handles the loading, management, and access of Vulkan textures, including
 * image data, image views, samplers, and image layouts.
 */
class BLOOM_API Texture {
  /**
   * @struct Dimensions
   * @brief Represents the dimensions of the texture.
   *
   * The Dimensions struct contains the width, height, and pixel size of the texture.
   */
  struct Dimensions {
    int width;
    int height;
    int pixelSize;
  };
public:
  Texture(Devices* device, const std::string &path);
  ~Texture();

  // Copy constructors
  Texture(const Texture&) = delete;
  Texture &operator=(const Texture&) = delete;

  // region Getters
  /**
   * @brief Gets the dimensions of the texture.
   * @return The Dimensions struct containing the texture dimensions.
   */
  Dimensions GetDimensions() const { return m_dimensions; }
  /**
   * @brief Gets the Vulkan sampler for the texture.
   * @return The Vulkan sampler.
   */
  VkSampler GetSampler() const { return m_sampler; }
  /**
   * @brief Gets the Vulkan image view for the texture.
   * @return The Vulkan image view.
   */
  VkImageView GetImageView() const { return m_imageView; }
  /**
   * @brief Gets the Vulkan image layout for the texture.
   * @return The Vulkan image layout.
   */
  VkImageLayout GetImageLayout() const { return m_imageLayout; }
private:
  /**
   * @brief Transitions the image layout of the texture.
   * @param oldLayout The old image layout.
   * @param newLayout The new image layout.
   */
  void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

  Devices* m_device;

  Dimensions m_dimensions;
  unsigned char* m_data; ///< Binary image data
  VkImage m_image;       ///< Vulkan image
  VkSampler m_sampler;   ///< Vulkan sampler
  VkDeviceMemory m_imageMemory;
  VkImageView m_imageView;
  VkFormat m_imageFormat;
  VkImageLayout m_imageLayout;
};

}
