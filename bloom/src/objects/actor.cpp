#include "actor.hpp"
#include "src/render/descriptors.hpp"
#include "src/render/swap_chain.hpp"

namespace bloom {

Actor::Actor(id_t id, render::Devices* devices) : Object(id, devices) {
  m_materialPool = render::DescriptorPool::Builder(*m_devices)
    .setMaxSets(render::SwapChain::MAX_FRAMES_IN_FLIGHT)
    .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, render::SwapChain::MAX_FRAMES_IN_FLIGHT * 4)
  .build();
}

void Actor::Begin() {}

void Actor::Tick(float deltaTime) { }
void Actor::End() {}
void Actor::LoadTextures(
    render::DescriptorSetLayout* descriptorSetLayout,
    const std::string &albedo,
    const std::string &rmo,
    const std::string &normal,
    const std::string &emissive) {
  m_textures[0] = std::make_unique<render::Texture>(m_devices, albedo);
  m_textures[1] = std::make_unique<render::Texture>(m_devices, rmo);
  m_textures[2] = std::make_unique<render::Texture>(m_devices, normal);
  m_textures[3] = std::make_unique<render::Texture>(m_devices, emissive);

  std::array<VkDescriptorImageInfo, 4> imageInfos{};
  for (int i = 0; i < 4; i++) {
    imageInfos[i].imageLayout = m_textures[i]->GetImageLayout();
    imageInfos[i].imageView = m_textures[i]->GetImageView();
    imageInfos[i].sampler = m_textures[i]->GetSampler();
  }

  render::DescriptorWriter(*descriptorSetLayout, *m_materialPool)
    .writeImage(0, &imageInfos[0])
    .writeImage(1, &imageInfos[1])
    .writeImage(2, &imageInfos[2])
    .writeImage(3, &imageInfos[3])
  .build(m_textureDescriptorSet);
}

} // namespace bloom
