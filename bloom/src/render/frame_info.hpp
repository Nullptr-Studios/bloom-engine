/**
 * @file frame_info.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 16/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <bloom_header.hpp>

namespace bloom::render {
struct FrameInfo {
  char frameIndex;
  float frameTime;
  VkCommandBuffer commandBuffer;
  Camera* activeCamera;
  VkDescriptorSet globalDescriptorSet;
};

}