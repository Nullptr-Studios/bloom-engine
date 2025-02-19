#include "model.hpp"

namespace bloom::render {

std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, position);
  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, texCoord);
  attributeDescriptions[2].binding = 0;
  attributeDescriptions[2].location = 2;
  attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attributeDescriptions[2].offset = offsetof(Vertex, color);
  return attributeDescriptions;
}

Model::Model(Devices* device, const std::vector<Vertex> &vertices) : m_device(device) {
  CreateVBO(vertices);
}

Model::~Model() {
  vkDestroyBuffer(m_device->device(), m_VBO, nullptr);
  vkFreeMemory(m_device->device(), m_VBOMemory, nullptr);
}

void Model::Bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {m_VBO};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Model::Draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
}

void Model::CreateVBO(const std::vector<Vertex> &vertices) {
  m_vertexCount = static_cast<unsigned int>(vertices.size());
  // Sanity check
  if (m_vertexCount < 3) {
    BLOOM_ERROR("Model requires at least 3 vertices, got {0}", m_vertexCount);
    BLOOM_INFO("Create VBO aborted");
    return;
  }
  VkDeviceSize bufferSize = sizeof(vertices[0]) * m_vertexCount;
  m_device->createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    m_VBO, m_VBOMemory);
  void* data;
  vkMapMemory(m_device->device(), m_VBOMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(m_device->device(), m_VBOMemory);
}

}
