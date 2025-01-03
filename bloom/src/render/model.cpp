#include "model.hpp"
#include "src/resource_loader.hpp"
#include <list>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

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
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, normal);
  attributeDescriptions[2].binding = 0;
  attributeDescriptions[2].location = 2;
  attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
  return attributeDescriptions;
}

void Model::Builder::LoadModel(const std::string &path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
    BLOOM_ERROR("Failed to load model {0}: {1}", path, err);
    return;
  }

  vertices.clear();
  indices.clear();

  for (const auto &shape : shapes) {
    for (const auto &index : shape.mesh.indices) {
      Vertex vertex{};

      if (index.vertex_index >= 0) {
        vertex.position = {
          attrib.vertices[3 * index.vertex_index + 0],
          attrib.vertices[3 * index.vertex_index + 1],
          attrib.vertices[3 * index.vertex_index + 2]
        };
      }

      if (index.normal_index >= 0) {
        vertex.normal = {
          attrib.normals[3 * index.normal_index + 0],
          attrib.normals[3 * index.normal_index + 1],
          attrib.normals[3 * index.normal_index + 2]
        };
      }

      if (index.texcoord_index >= 0) {
        vertex.texCoord = {
          attrib.texcoords[2 * index.texcoord_index + 0],
          attrib.texcoords[2 * index.texcoord_index + 1]
        };
      }

      vertices.push_back(vertex);
      indices.push_back(static_cast<unsigned short>(indices.size()));
    }
  }
}

Model::Model(const Builder &builder) {
  CreateVBO(builder.vertices);
  CreateEBO(builder.indices);
}

// region GLTF Loader
// This is the start of a .gltf loader -x
// Since we dont need it right now, i'm leaving it commented until .obj files are no longer enough
// I made some graphs detailing the binary file structure and what each part of the json file means, if
// you want to see them, let me know -x
// Model::Model(const char* path) : m_path(path) {
//   // Get data from the Binary file
//   std::string gltf = ResourceLoader::LoadFileAsync(path).get();
//   json gltfJson = json::parse(gltf);
//   auto data = GetData(gltfJson);
//   // Separate the buffers
//   auto gltfBuffers = GetBuffers(gltfJson, data);
//   // Serialize the buffers
//   Builder builder = BuildVertices(gltfJson, gltfBuffers);
//   // Create VBO and EBO
//   CreateVBO(builder.vertices);
//   CreateEBO(builder.indices);
// }
// std::vector<unsigned char> Model::GetData(json gltfJson) {
//   std::string uri = gltfJson["buffers"][0]["uri"];
//   std::string fileDir = std::string(m_path).substr(0, std::string(m_path).find_last_of('/') + 1);
//   std::string bytesText = ResourceLoader::LoadFileAsync(fileDir + uri).get();
//   std::vector<unsigned char> bytes(bytesText.begin(), bytesText.end());
//   return bytes;
// }
//
// std::vector<std::vector<unsigned char>> Model::GetBuffers(json gltfJson, std::vector<unsigned char> data) {
//   auto bufferViews = gltfJson["bufferViews"];
//   unsigned int size = bufferViews.size();
//   std::vector<std::vector<unsigned char>> buffers(size);
//   for (unsigned int i = 0; i < size; i++) {
//     auto bufferView = bufferViews[i];
//     unsigned int byteOffset = bufferView["byteOffset"];
//     unsigned int byteLength = bufferView["byteLength"];
//     buffers[i].resize(byteLength);
//     std::copy(data.begin() + byteOffset, data.begin() + byteOffset + byteLength, buffers[i].begin());
//   }
//   return buffers;
// }
//
// std::vector<float> GetFloats(nlohmann::basic_json<>::const_reference accessor, std::vector<unsigned char> &buffer) {
//   unsigned int count = accessor["count"];
//   std::string type = accessor["type"].dump();
//   if (count * sizeof(float) != sizeof(buffer)) {
//     BLOOM_ERROR("Invalid byte length for accessor, expected {0}, got {1}", count * sizeof(float), sizeof(buffer));
//     return std::vector<float>();
//   }
//   BLOOM_INFO("Copying {0} bytes from buffer of type {1} to floats", sizeof(buffer), type);
//   std::vector<float> floats(count);
//   std::memcpy(floats.data(), buffer.data(), sizeof(buffer));
//   return floats;
// }
// std::vector<unsigned short> GetIndices(nlohmann::basic_json<>::const_reference accessors,
//                                        const std::vector<std::vector<unsigned char>> & vector) {
//
// }
//
// Model::Builder Model::BuildVertices(json gltfJson, std::vector<std::vector<unsigned char>> gltfBuffers) {
//   unsigned int size = gltfJson["accessors"].size();
//   for (unsigned int i = 0; i < size; i++) {
//     if (gltfJson["accessors"][i]["type"] == "VEC3") {
//       auto floats =
//     }
//   }
//
//   Builder builder = MakeBuilder(floats, indices);
// }

Model::~Model() {}

std::unique_ptr<Model> Model::CreateModel(const std::string &path) {
  Builder builder;
  builder.LoadModel(path);
  BLOOM_INFO("Model {0} loaded correctly with {1} vertices", path, builder.vertices.size());
  return std::make_unique<Model>(builder);
}

void Model::Bind(VkCommandBuffer commandBuffer) const {
  VkBuffer buffers[] = {m_VBO->GetBuffer()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

  if (m_hasIndices)
    vkCmdBindIndexBuffer(commandBuffer, m_EBO->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
}

void Model::Draw(VkCommandBuffer commandBuffer) const {
  if (m_hasIndices)
    vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
  else
    vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
}

// region Create Buffers
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
    *BLOOM_DEVICES,
    vertexSize,
    m_vertexCount,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
  );

  stagingBuffer.Map();
  stagingBuffer.WriteToBuffer((void*)vertices.data());

  m_VBO = std::make_unique<Buffer>(
    *BLOOM_DEVICES,
    vertexSize,
    m_vertexCount,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
  );

  BLOOM_DEVICES->CopyBuffer(stagingBuffer.GetBuffer(), m_VBO->GetBuffer(), bufferSize);
}

void Model::CreateEBO(const std::vector<unsigned short>& indices) {
  m_indexCount = static_cast<unsigned short>(indices.size());
  m_hasIndices = m_indexCount > 0;
  if (!m_hasIndices) return;
  VkDeviceSize bufferSize = sizeof(indices[0]) * m_indexCount;
  unsigned int indexSize = sizeof(indices[0]);

  Buffer stagingBuffer(
    *BLOOM_DEVICES,
    indexSize,
    m_indexCount,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
  );

  stagingBuffer.Map();
  stagingBuffer.WriteToBuffer((void*)indices.data());

  m_EBO = std::make_unique<Buffer>(
    *BLOOM_DEVICES,
    indexSize,
    m_indexCount,
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
  );
  BLOOM_DEVICES->CopyBuffer(stagingBuffer.GetBuffer(), m_EBO->GetBuffer(), bufferSize);
}

}
