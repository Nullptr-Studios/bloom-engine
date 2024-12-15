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
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec4 color;

    static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
  };

  struct Builder {
    std::vector<Vertex> vertices;
    // unsigned short will limit the number of vertices to 65,535 -x
    // unsigned int will limit the number of vertices to 4,294,967,295 -x
    // Remember to update the bind function -x
    std::vector<unsigned short> indices;
  };

  Model(Devices* device, const Builder& builder);
  ~Model();

  Model(const Model&) = delete;
  Model &operator=(const Model&) = delete;

  void Bind(VkCommandBuffer commandBuffer);
  void Draw(VkCommandBuffer commandBuffer);

private:
  Devices* m_device;

  // VBO stuff
  void CreateVBO(const std::vector<Vertex> &vertices);
  VkBuffer m_VBO;
  VkDeviceMemory m_VBOMemory;
  unsigned int m_vertexCount;

  // EBO stuff
  void CreateEBO(const std::vector<unsigned short> &indices);
  bool m_hasIndices;
  VkBuffer m_EBO;
  VkDeviceMemory m_EBOMemory;
  unsigned short m_indexCount;
};

}
