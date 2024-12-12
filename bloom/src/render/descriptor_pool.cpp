#include "descriptor_pool.hpp"

namespace bloom::render {

DescriptorPool::DescriptorPool(Devices* devices, uint32_t maxSets) : devices(devices){
  std::array<VkDescriptorPoolSize, 1> poolSizes{};
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[0].descriptorCount = maxSets;

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = maxSets;

  if (vkCreateDescriptorPool(devices->device(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

DescriptorPool::~DescriptorPool() {
  vkDestroyDescriptorPool(devices->device(), descriptorPool, nullptr);
}

}