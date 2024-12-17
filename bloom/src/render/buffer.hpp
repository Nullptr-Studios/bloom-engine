/**
 * @file buffer.hpp
 * @author Brendan Galea
 * @date 15/12/2024
 *
 * @brief Vulkan Buffer abstraction file
 */

#pragma once
#include "devices.hpp"

namespace bloom::render {

class Buffer {
public:
  Buffer(
    Devices& device,
    VkDeviceSize instanceSize,
    uint32_t instanceCount,
    VkBufferUsageFlags usageFlags,
    VkMemoryPropertyFlags memoryPropertyFlags,
    VkDeviceSize minOffsetAlignment = 1
  );
  ~Buffer();

  // Copy constructors
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  /**
   * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
   *
   * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
   * buffer range.
   * @param offset (Optional) Byte offset from beginning
   *
   * @return VkResult of the buffer mapping call
   */
  VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
  /**
   * Unmap a mapped memory range
   *
   * @note Does not return a result as vkUnmapMemory can't fail
   */
  void Unmap();

  /**
   * Copies the specified data to the mapped buffer. Default value writes whole buffer range
   *
   * @param data Pointer to the data to copy
   * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
   * range.
   * @param offset (Optional) Byte offset from beginning of mapped region
   *
   */
  void WriteToBuffer(const void * data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
  /**
   * Flush a memory range of the buffer to make it visible to the device
   *
   * @note Only required for non-coherent memory
   *
   * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
   * complete buffer range.
   * @param offset (Optional) Byte offset from beginning
   *
   * @return VkResult of the flush call
   */
  VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
  /**
   * Create a buffer info descriptor
   *
   * @param size (Optional) Size of the memory range of the descriptor
   * @param offset (Optional) Byte offset from beginning
   *
   * @return VkDescriptorBufferInfo of specified offset and range
   */
  VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;
  /**
   * Invalidate a memory range of the buffer to make it visible to the host
   *
   * @note Only required for non-coherent memory
   *
   * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
   * the complete buffer range.
   * @param offset (Optional) Byte offset from beginning
   *
   * @return VkResult of the invalidate call
   */
  VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) const;

  /**
   * Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
   *
   * @param data Pointer to the data to copy
   * @param index Used in offset calculation
   *
   */
  void WriteToIndex(const void * data, int index) const;
  /**
   *  Flush the memory range at index * alignmentSize of the buffer to make it visible to the device
   *
   * @param index Used in offset calculation
   *
   */
  VkResult FlushIndex(int index) const;
  /**
   * Create a buffer info descriptor
   *
   * @param index Specifies the region_ given by index x alignmentSize
   *
   * @return VkDescriptorBufferInfo for instance at index
   */
  VkDescriptorBufferInfo DescriptorInfoForIndex(int index) const;
  /**
   * Invalidate a memory range of the buffer to make it visible to the host
   *
   * @note Only required for non-coherent memory
   *
   * @param index Specifies the region_ to invalidate: index x alignmentSize
   *
   * @return VkResult of the invalidate call
   */
  VkResult InvalidateIndex(int index) const;
 
/**
 * @brief Gets the underlying Vulkan buffer handle.
 *
 * This handle represents the Vulkan buffer object that stores the data.  It's
 * used by Vulkan for various operations like binding the buffer to a command buffer
 * and transferring data.
 *
 * @return The Vulkan buffer handle.
 */
VkBuffer GetBuffer() const { return m_buffer; }
/**
 * @brief Gets a pointer to the mapped memory of the buffer.
 *
 * This provides direct access to the CPU-accessible memory region_ where the buffer's
 * data resides.  This pointer can be used to write or read data directly from the buffer.
 *
 * @return A pointer to the mapped memory.  This pointer will be `nullptr` if
 *         the buffer is not mapped.
 */
void* GetMappedMemory() const { return m_mapped; }
/**
 * @brief Gets the number of instances stored in the buffer.
 *
 * This represents the number of individual data elements (instances) contained
 * within the buffer.  For example, if the buffer stores vertex data, this would
 * be the number of vertices.
 *
 * @return The instance count.
 */
uint32_t GetInstanceCount() const { return m_instanceCount; }
/**
 * @brief Gets the size of a single instance in the buffer.
 *
 * This represents the size, in bytes, of each individual data element within the buffer.
 *
 * @return The size of an instance in bytes.
 */
VkDeviceSize GetInstanceSize() const { return m_instanceSize; }
/**
 * @brief Gets the alignment size used for instances in the buffer.
 *
 * This represents the alignment requirement, in bytes, for each instance in the buffer.
 * This value is likely the same as the instance size, but it's important to distinguish
 * between them.  It is recommended to review whether a separate alignment member
 * is needed, or if this function should simply return the instance size.  This function
 * currently returns the same value as `GetInstanceSize()`.
 *
 * @return The alignment size in bytes.
 */
VkDeviceSize GetAlignmentSize() const { return m_instanceSize; } // Consider using a dedicated m_alignmentSize member if alignment differs from instance size.
/**
 * @brief Gets the usage flags specified for the buffer.
 *
 * These flags define how the buffer will be used by Vulkan (e.g., vertex buffer,
 * index buffer, uniform buffer, etc.).
 *
 * @return The buffer usage flags.  These are Vulkan `VkBufferUsageFlagBits` values.
 */
VkBufferUsageFlags GetUsageFlags() const { return m_usageFlags; }
/**
 * @brief Gets the memory property flags specified for the buffer.
 *
 * These flags define the memory properties of the buffer (e.g., host visible,
 * host coherent, device local, etc.).
 *
 * @return The memory property flags. These are Vulkan `VkMemoryPropertyFlagBits` values.
 */
VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return m_memoryPropertyFlags; }
/**
 * @brief Gets the total size of the buffer in bytes.
 *
 * This represents the total allocated size of the buffer.
 *
 * @return The buffer size.
 */
VkDeviceSize GetBufferSize() const { return m_bufferSize; }
 
private:
  /**
   *  Returns the minimum instance size required to be compatible with devices minOffsetAlignment
   *
   *  @param instanceSize The size of an instance
   *  @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (e.g.
   *  minUniformBufferOffsetAlignment)
   *
   *  @return VkResult of the buffer mapping call
   */
  static VkDeviceSize GetAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);
 
  Devices& m_devices;
  void* m_mapped = nullptr;
  VkBuffer m_buffer = VK_NULL_HANDLE;
  VkDeviceMemory m_memory = VK_NULL_HANDLE;
 
  VkDeviceSize m_bufferSize;
  uint32_t m_instanceCount;
  VkDeviceSize m_instanceSize;
  VkDeviceSize m_alignmentSize;
  VkBufferUsageFlags m_usageFlags;
  VkMemoryPropertyFlags m_memoryPropertyFlags;
};

}
