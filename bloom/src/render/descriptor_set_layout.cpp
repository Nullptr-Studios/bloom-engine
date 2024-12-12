#include "descriptor_set_layout.hpp"

namespace bloom::render {

DescriptorSetLayout::DescriptorSetLayout(Devices* devices, const std::vector<VkDescriptorSetLayoutBinding>& bindings) : m_device(devices) {
  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  if (vkCreateDescriptorSetLayout(devices->device(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
    BLOOM_CRITICAL("failed to create descriptor set layout!");
  }
}

DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(m_device->device(), m_descriptorSetLayout, nullptr);
}

}