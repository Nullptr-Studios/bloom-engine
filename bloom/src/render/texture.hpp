/**
 * \file Texture.h
 * \author Xein <xgonip@gmail.com>
 * \date 11/12/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once
#include "devices.hpp"

namespace bloom::render {

class BLOOM_API Texture {
  struct Dimensions {
    int width;
    int height;
    int pixelSize;
  };
public:
  Texture(Devices* device, const std::string &path);
  ~Texture();

  Texture(const Texture&) = delete;
  Texture &operator=(const Texture&) = delete;

  Dimensions GetDimensions() const { return m_dimensions; }
  VkSampler GetSampler() const { return m_sampler; }
  VkImageView GetImageView() const { return m_imageView; }
  VkImageLayout GetImageLayout() const { return m_imageLayout; }
private:
  void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);

  Devices* m_device;

  unsigned char* m_data;
  Dimensions m_dimensions;
  VkImage m_image;
  VkDeviceMemory m_imageMemory;
  VkImageView m_imageView;
  VkSampler m_sampler;
  VkFormat m_imageFormat;
  VkImageLayout m_imageLayout;
};

}
