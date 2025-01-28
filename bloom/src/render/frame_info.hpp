/**
 * @file frame_info.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 2024-12-16
 *
 * @brief Defines the FrameInfo struct
 */

#pragma once

namespace bloom {
class Camera;

struct DirectionalLightInfo {
  glm::vec4 direction = glm::vec4(0.0f); ///< XYZ direction of the light
  glm::vec4 color = glm::vec4(1.0f); ///< RGB color of the light, A is intensity
};

/**
 *  @macro MAX_LIGHTS
 *  @brief Defines the maximum light sources that can be rendered in a single frame
 */
#define MAX_LIGHTS 16 // We will probably need to update this later on -x

/**
 *  @struct PointLightInfo
 *  @brief Represents a point light source in the scene.
 *
 *  This structure defines all the necessary parameters for a point light source
 */
struct PointLightInfo {
  glm::vec4 position = glm::vec4(0.0f); ///< XYZ position of the light, W is radius
  glm::vec4 color = glm::vec4(1.0f); ///< RGB color of the light, A is intensity
};

struct GlobalUBO {
  glm::mat4 projectionMatrix = glm::mat4(1.0f);
  glm::vec4 indirectColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.02f);
  DirectionalLightInfo directionalLight;
  PointLightInfo pointLights[MAX_LIGHTS];
  int lightCount = 0;
};

namespace render {

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
}
