/**
 * @file Descriptors.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 16/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "devices.hpp"

namespace bloom::render {

class BLOOM_API DescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(Devices &device) : m_device{device} {}

    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<DescriptorSetLayout> build() const;

   private:
    Devices &m_device;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings{};
  };

  DescriptorSetLayout(
      Devices &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~DescriptorSetLayout();
  DescriptorSetLayout(const DescriptorSetLayout &) = delete;
  DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }

 private:
  Devices &m_device;
  VkDescriptorSetLayout m_descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

  friend class DescriptorWriter;
};

class BLOOM_API DescriptorPool {
 public:
  class Builder {
   public:
    Builder(Devices &device) : m_device{device} {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<DescriptorPool> build() const;

   private:
    Devices &m_device;
    std::vector<VkDescriptorPoolSize> m_poolSizes{};
    uint32_t m_maxSets = 1000;
    VkDescriptorPoolCreateFlags m_poolFlags = 0;
  };

  DescriptorPool(
      Devices &device,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~DescriptorPool();
  DescriptorPool(const DescriptorPool &) = delete;
  DescriptorPool &operator=(const DescriptorPool &) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();

 private:
  Devices &m_device;
  VkDescriptorPool m_descriptorPool;

  friend class DescriptorWriter;
};

class BLOOM_API DescriptorWriter {
 public:
  DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool);

  DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

 private:
  DescriptorSetLayout &m_setLayout;
  DescriptorPool &m_pool;
  std::vector<VkWriteDescriptorSet> m_writes;
};

}
