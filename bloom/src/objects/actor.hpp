/**
 * @file actor.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 15/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#define ALBEDO_PLACEHOLDER "resources/textures/T_Default.tga"
#define RMO_PLACEHOLDER "resources/textures/T_Default_RMO.tga"
#define NORMAL_PLACEHOLDER "resources/textures/T_Default_N.tga"
#define EMISSIVE_PLACEHOLDER "resources/textures/T_Default_E.tga"

#include "object.hpp"
#include "src/render/descriptors.hpp"

namespace bloom {

class BLOOM_API Actor : public Object{

public:
  explicit Actor(id_t id, render::Devices* devices);
  ~Actor() = default;
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;
  Actor(Actor&&) = default;
  Actor& operator=(Actor&&) = default;

  void Begin() override;
  void Tick(float deltaTime) override;
  void End() override;

  void LoadTextures(
    render::DescriptorSetLayout* descriptorSetLayout,
    const std::string& albedo = ALBEDO_PLACEHOLDER,
    const std::string& rmo = RMO_PLACEHOLDER,
    const std::string& normal = NORMAL_PLACEHOLDER,
    const std::string& emissive = EMISSIVE_PLACEHOLDER
  );

  VkDescriptorSet GetDescriptorSet() const { return m_textureDescriptorSet; }

  std::shared_ptr<render::Model> model;
  glm::vec3 tintColor;
  render::Texture* texture;
private:
  std::unique_ptr<render::DescriptorPool> m_materialPool;
  std::array<std::unique_ptr<render::Texture>, 4> m_textures;
  VkDescriptorSet m_textureDescriptorSet;
};

typedef std::unordered_map<id_t,std::shared_ptr<Actor>> ActorMap;

}

#undef ALBEDO_PLACEHOLDER
#undef RMO_PLACEHOLDER
#undef NORMAL_PLACEHOLDER
#undef EMISSIVE_PLACEHOLDER