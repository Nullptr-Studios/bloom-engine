#include "buffer.hpp"

namespace bloom::render {

VkDeviceSize Buffer::GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
  if (minOffsetAlignment > 0)
    return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
  return instanceSize;
}
 
Buffer::Buffer(
    Devices& device,
    VkDeviceSize instanceSize,
    uint32_t instanceCount,
    VkBufferUsageFlags usageFlags,
    VkMemoryPropertyFlags memoryPropertyFlags,
    VkDeviceSize minOffsetAlignment)
    : m_devices{device},
      m_instanceCount{instanceCount},
      m_instanceSize{instanceSize},
      m_usageFlags{usageFlags},
      m_memoryPropertyFlags{memoryPropertyFlags} {
  m_alignmentSize = GetAlignment(instanceSize, minOffsetAlignment);
  m_bufferSize = m_alignmentSize * instanceCount;
  device.createBuffer(m_bufferSize, usageFlags, memoryPropertyFlags, m_buffer, m_memory);
}
 
Buffer::~Buffer() {
  Unmap();
  vkDestroyBuffer(m_devices.device(), m_buffer, nullptr);
  vkFreeMemory(m_devices.device(), m_memory, nullptr);
}

VkResult Buffer::Map(VkDeviceSize size, VkDeviceSize offset) {
  assert(m_buffer && m_memory && "Called map on buffer before create");
  return vkMapMemory(m_devices.device(), m_memory, offset, size, 0, &m_mapped);
}

void Buffer::Unmap() {
  if (m_mapped) {
    vkUnmapMemory(m_devices.device(), m_memory);
    m_mapped = nullptr;
  }
}

void Buffer::WriteToBuffer(const void *data, VkDeviceSize size, VkDeviceSize offset) const {
  assert(m_mapped && "Cannot copy to unmapped buffer");
 
  if (size == VK_WHOLE_SIZE)
    memcpy(m_mapped, data, m_bufferSize);
  else {
    char *memOffset = static_cast<char *>(m_mapped);
    memOffset += offset;
    memcpy(memOffset, data, size);
  }
}

VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset) const {
  VkMappedMemoryRange mappedRange = {};
  mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  mappedRange.memory = m_memory;
  mappedRange.offset = offset;
  mappedRange.size = size;
  return vkFlushMappedMemoryRanges(m_devices.device(), 1, &mappedRange);
}

VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset) const {
  VkMappedMemoryRange mappedRange = {};
  mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
  mappedRange.memory = m_memory;
  mappedRange.offset = offset;
  mappedRange.size = size;
  return vkInvalidateMappedMemoryRanges(m_devices.device(), 1, &mappedRange);
}

VkDescriptorBufferInfo Buffer::DescriptorInfo(VkDeviceSize size, VkDeviceSize offset) const {
  return VkDescriptorBufferInfo{
      m_buffer,
      offset,
      size,
  };
}

void Buffer::WriteToIndex(const void *data, int index) const {
  WriteToBuffer(data, m_instanceSize, index * m_alignmentSize);
}

VkResult Buffer::FlushIndex(int index) const { return Flush(m_alignmentSize, index * m_alignmentSize); }

VkDescriptorBufferInfo Buffer::DescriptorInfoForIndex(int index) const {
  return DescriptorInfo(m_alignmentSize, index * m_alignmentSize);
}

VkResult Buffer::InvalidateIndex(int index) const {
  return Invalidate(m_alignmentSize, index * m_alignmentSize);
}

}