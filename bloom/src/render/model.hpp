/**
 * @file model.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 29/11/2024
 *
 * @brief [Insert file description]
 */

#pragma once
#include "devices.hpp"
#include <bloom_header.hpp>

namespace bloom::render {

class BLOOM_API Model {
public:
  struct Vertex {
    glm::vec2 position;
    glm::vec4 color;

    static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
  };

  Model(Devices* device, const std::vector<Vertex> &vertices);
  ~Model();

  Model(const Model&) = delete;
  Model &operator=(const Model&) = delete;

  void Bind(VkCommandBuffer commandBuffer);
  void Draw(VkCommandBuffer commandBuffer);

private:
  void CreateVBO(const std::vector<Vertex> &vertices);

  Devices* m_device;
  VkBuffer m_VBO;
  VkDeviceMemory m_VBOMemory;
  unsigned int m_vertexCount;
};

}
