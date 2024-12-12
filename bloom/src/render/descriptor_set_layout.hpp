/**
 * \file descriptor_set_layout.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 12/12/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once
#include "devices.hpp"

namespace bloom::render {

class BLOOM_API DescriptorSetLayout {
public:
  DescriptorSetLayout(Devices* devices, const std::vector<VkDescriptorSetLayoutBinding>& bindings);
  ~DescriptorSetLayout();

  VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }

private:
  Devices* m_device;
  VkDescriptorSetLayout m_descriptorSetLayout;
};

}
