#include "renderer.hpp"

namespace bloom::render {

std::vector<VkVertexInputBindingDescription> Renderer::Vertex::GetBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Renderer::Vertex::GetAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = 0;
  return attributeDescriptions;
}

Renderer::Renderer(Devices* device, const std::vector<Vertex> &vertices) : m_device(device) {
  CreateVBO(vertices);
}

Renderer::~Renderer() {
  vkDestroyBuffer(m_device->device(), m_VBO, nullptr);
  vkFreeMemory(m_device->device(), m_VBOMemory, nullptr);
}

void Renderer::Bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {m_VBO};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Renderer::Draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
}

void Renderer::CreateVBO(const std::vector<Vertex> &vertices) {
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
