/**
 * @file model.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 29/11/2024
 *
 * @brief Defines the Model class used for rendering in the bloom engine.
 *
 * This file contains the declaration of the Model class, which is responsible for managing
 * vertex and index buffers, and providing methods to bind and draw the model using Vulkan.
 */

#pragma once
#include "devices.hpp"
#include "buffer.hpp"
#include <bloom_header.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace bloom::render {

/**
 * @class Model
 * @brief Represents a 3D model in the bloom rendering engine.
 *
 * The Model class encapsulates the vertex and index buffers, and provides methods to bind
 * and draw the model using Vulkan command buffers.
 */
class BLOOM_API Model {
public:
  /**
   * @struct Vertex
   * @brief Represents a single vertex in the model.
   *
   * The Vertex struct contains position, texture coordinate, and color information for a vertex.
   */
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();

    bool operator==(const Vertex& other) const {
      return position == other.position && normal == other.normal && texCoord == other.texCoord;
    }
  };

  /**
   * @struct Builder
   * @brief Helper struct to build a Model instance.
   *
   * The Builder struct contains vectors of vertices and indices used to construct a Model.
   */
  struct Builder {
    std::vector<Vertex> vertices;
    // unsigned short will limit the number of vertices to 65,535 -x
    // unsigned int will limit the number of vertices to 4,294,967,295 -x
    // Remember to update the bind function -x
    std::vector<unsigned short> indices;

    void LoadModel(const std::string& path);
  };

  Model(const Builder& builder);
  ~Model();

  // Copy constructors
  Model(const Model&) = delete;
  Model &operator=(const Model&) = delete;

  static std::unique_ptr<Model> LoadOBJ(const std::string& path);

  /**
   * @brief Binds the model's vertex and index buffers to the command buffer.
   * @param commandBuffer The command buffer to bind the buffers to.
   */
  void Bind(VkCommandBuffer commandBuffer) const;
  /**
   * @brief Draws the model using the bound buffers and command buffer.
   * @param commandBuffer The command buffer to record the draw command to.
   */
  void Draw(VkCommandBuffer commandBuffer) const;

private:
  const char* m_path;

  // gltf loader stuff
  // std::vector<unsigned char> GetData(json gltfJson);
  // std::vector<std::vector<unsigned char>> GetBuffers(json gltfJson, std::vector<unsigned char> data);
  // Builder BuildVertices(json gltfJson, std::vector<std::vector<unsigned char>> gltfBuffers);

  // region VBO stuff
  /**
   * @brief Creates the vertex buffer object (VBO).
   * @param vertices The vertex data for the model.
   */
  void CreateVBO(const std::vector<Vertex> &vertices);
  std::unique_ptr<Buffer> m_VBO;
  unsigned int m_vertexCount;

  // region EBO stuff
  /**
   * @brief Creates the index buffer object (EBO).
   * @param indices The index data for the model.
   */
  void CreateEBO(const std::vector<unsigned short> &indices);
  bool m_hasIndices;
  std::unique_ptr<Buffer> m_EBO;
  unsigned short m_indexCount;
};

}
