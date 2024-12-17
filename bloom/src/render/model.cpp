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

Model::Model(Devices* device, const Builder& builder) : m_device(device) {
  CreateVBO(builder.vertices);
  CreateEBO(builder.indices);
}

Model::~Model() { }

void Model::Bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {m_VBO->GetBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

  if (m_hasIndices) {
    vkCmdBindIndexBuffer(commandBuffer, m_EBO->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
  }
}

void Model::Draw(VkCommandBuffer commandBuffer) {
  if (m_hasIndices) {
    vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
  } else {
    vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
  }
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
  unsigned int vertexSize = sizeof(vertices[0]);

  Buffer stagingBuffer(
    *m_device,
    vertexSize,
    m_vertexCount,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
  );

  stagingBuffer.Map();
  stagingBuffer.WriteToBuffer((void*)vertices.data());

  m_VBO = std::make_unique<Buffer>(
    *m_device,
    vertexSize,
    m_vertexCount,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
  );

  m_device->copyBuffer(stagingBuffer.GetBuffer(), m_VBO->GetBuffer(), bufferSize);
}

void Model::CreateEBO(const std::vector<unsigned short>& indices) {
  m_indexCount = static_cast<unsigned short>(indices.size());
  m_hasIndices = m_indexCount > 0;
  if (!m_hasIndices) return;
  VkDeviceSize bufferSize = sizeof(indices[0]) * m_indexCount;
  unsigned int indexSize = sizeof(indices[0]);

  Buffer stagingBuffer(
    *m_device,
    indexSize,
    m_indexCount,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
  );

  stagingBuffer.Map();
  stagingBuffer.WriteToBuffer((void*)indices.data());

  m_EBO = std::make_unique<Buffer>(
    *m_device,
    indexSize,
    m_indexCount,
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
  );
  m_device->copyBuffer(stagingBuffer.GetBuffer(), m_EBO->GetBuffer(), bufferSize);
}

}
