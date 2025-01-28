#include "actor.hpp"
#include "src/engine.hpp"
#include "src/render/buffer.hpp"
#include "src/render/descriptor_layouts.hpp"
#include "src/render/descriptors.hpp"
#include "src/render/texture.hpp"

namespace bloom {

Actor::Actor(id_t id) : Object(id) {
  m_materialPool =
    render::DescriptorPool::Builder(*render::Devices::GetInstance())
      .SetMaxSets(1)
      .SetPoolFlags(VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT)
      .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1)
      .AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4)
    .Build();

  m_materialBuffer = new render::Buffer(*BLOOM_DEVICES, sizeof(MaterialProperties), 1,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
  );
  m_materialBuffer->Map();
}

Actor::~Actor() {
  m_materialBuffer->Unmap();
  delete m_materialBuffer;
}

void Actor::PropertiesPanel() {
  Object::PropertiesPanel();

  if (ImGui::CollapsingHeader("Actor")) {
    ImGui::ColorEdit4("Tint Color", &materialProperties.tint.r);
    ImGui::Checkbox("Has Albedo", &materialProperties.hasAlbedoTexture);
    ImGui::Checkbox("Has RMO", &materialProperties.hasRmoTexture);
    if (!materialProperties.hasRmoTexture) {
      ImGui::SliderFloat("Roughness", &materialProperties.roughness, 0.0f, 1.0f);
      ImGui::SliderFloat("Metallic", &materialProperties.metallic, 0.0f, 1.0f);
      ImGui::DragFloat("Ambient Occlusion Strength", &materialProperties.occlusionStrength, 0.1f);
    }
    ImGui::Checkbox("Has Normals", &materialProperties.hasNormals);
    ImGui::Checkbox("Has Emission", &materialProperties.hasEmission);
    if (materialProperties.hasEmission)
      ImGui::DragFloat("Emission Strength", &materialProperties.emissionStrength);
    ImGui::Text("This doesn't work, we should implement it ASAP");
    if (ImGui::Button("Rebuild Material")) {
      // TODO: This crashes the engine, we should look into it ASAP
      // RebuildDescriptorSet();
    }
  }
}

void Actor::LoadTextures(const std::string &albedo, const std::string &rmo,
    const std::string &normal, const std::string &emission) {
  m_textures[0] = std::make_unique<render::Texture>(render::Devices::GetInstance(), albedo);
  m_textures[1] = std::make_unique<render::Texture>(render::Devices::GetInstance(), rmo);
  m_textures[2] = std::make_unique<render::Texture>(render::Devices::GetInstance(), normal);
  m_textures[3] = std::make_unique<render::Texture>(render::Devices::GetInstance(), emission);

  std::array<VkDescriptorImageInfo, 4> imageInfos{};
  for (int i = 0; i < 4; i++) {
    imageInfos[i].imageLayout = m_textures[i]->GetImageLayout();
    imageInfos[i].imageView = m_textures[i]->GetImageView();
    imageInfos[i].sampler = m_textures[i]->GetSampler();
  }

  auto bufferInfo = m_materialBuffer->DescriptorInfo();
  m_materialBuffer->WriteToBuffer(&materialProperties);

  render::DescriptorWriter(*Engine::GetDescriptorLayouts().materialLayout, *m_materialPool)
    .WriteBuffer(0, &bufferInfo)
    .WriteImage(1, &imageInfos[0])
    .WriteImage(2, &imageInfos[1])
    .WriteImage(3, &imageInfos[2])
    .WriteImage(4, &imageInfos[3])
  .Build(m_textureDescriptorSet);
}

void Actor::RebuildDescriptorSet() {
  std::array<VkDescriptorImageInfo, 4> imageInfos{};
  for (int i = 0; i < 4; i++) {
    imageInfos[i].imageLayout = m_textures[i]->GetImageLayout();
    imageInfos[i].imageView = m_textures[i]->GetImageView();
    imageInfos[i].sampler = m_textures[i]->GetSampler();
  }

  auto bufferInfo = m_materialBuffer->DescriptorInfo();
  m_materialBuffer->WriteToBuffer(&materialProperties);

  render::DescriptorWriter(*Engine::GetDescriptorLayouts().materialLayout, *m_materialPool)
    .WriteBuffer(0, &bufferInfo)
    .WriteImage(1, &imageInfos[0])
    .WriteImage(2, &imageInfos[1])
    .WriteImage(3, &imageInfos[2])
    .WriteImage(4, &imageInfos[3])
  .Overwrite(m_textureDescriptorSet);
}

}
