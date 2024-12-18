/**
 * @file frame_info.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 2024-12-16
 *
 * @brief Defines the FrameInfo struct
 */

#pragma once
#include "src/engine.hpp"
#include "src/objects/camera.hpp"

namespace bloom::render {

/**
 *  @struct FrameInfo
 *  @brief Per-frame information.
 *
 *  This structure holds various data related to a specific frame. This data
 *  facilitates efficient rendering and updates within each frame.
 */
struct FrameInfo {
  char frameIndex;                     ///< Index of the current frame
  float frameTime;                     ///< Time elapsed since the last frame in seconds
  VkCommandBuffer commandBuffer;       ///< The command buffer for the current frame
  Camera* activeCamera;                ///< Pointer to the currently active camera
  VkDescriptorSet globalDescriptorSet; ///< Global descriptor set for the current frame
};

}
