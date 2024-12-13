/**
 * \file descriptor_pool.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 12/12/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once
#include "devices.hpp"

namespace bloom::render {

class BLOOM_API DescriptorPool {
public:
  DescriptorPool(Devices* devices, uint32_t maxSets);
  ~DescriptorPool();

  VkDescriptorPool getDescriptorPool() const { return descriptorPool; }

private:
  Devices* devices;
  VkDescriptorPool descriptorPool;
};

}
