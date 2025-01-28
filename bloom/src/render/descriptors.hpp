/**
 * @file Descriptors.hpp
 * @author Xein 
 * @date 16/12/2024
 *
 * @brief Vulkan descriptor management helper classes
 */

#pragma once
#include "devices.hpp"

namespace bloom::render {

/**
 * @class DescriptorSetLayout
 * @brief Defines the layout of descriptors in a shader.
 *
 * This class represents a Vulkan descriptor set layout, which serves as a blueprint for
 * creating descriptor sets. It specifies the types and bindings of descriptors that can be
 * accessed by a shader.  For example, it defines whether a descriptor is a uniform buffer,
 * a sampled image, or another type, and at which binding point in the shader it's accessible.
 * Multiple descriptor sets can be created from the same layout, ensuring consistency
 * between the shader and the data it uses.  This layout is crucial for binding resources
 * like buffers and images to the shader pipeline.
 */
class BLOOM_API DescriptorSetLayout {
public:
  /**
   * @class Builder
   * @brief Helper class to build DescriptorSetLayouts.
   *
   * The Builder class provides a fluent interface for constructing DescriptorSetLayout objects.
   * This approach simplifies the creation process by allowing chained calls to the AddBinding
   * function. It also improves code readability and maintainability by encapsulating the
   * layout creation logic.
   */
  class Builder {
  public:
    /**
     * @brief Constructs a Builder object.
     *
     * @param device A reference to the Devices object, providing access to the Vulkan device.
     *        This is necessary for creating the descriptor set layout.
     */
    explicit Builder(Devices &device) : m_device{device} {}

    /**
     * @brief Adds a descriptor binding to the layout.
     *
     * @param binding The binding point in the shader.
     * @param descriptorType The type of the descriptor (e.g., uniform buffer, combined image sampler).
     * @param stageFlags The shader stages that will access this descriptor.
     * @param count The number of descriptors in the array.  Defaults to 1.
     */
    Builder &AddBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    /**
     * @brief Builds the DescriptorSetLayout object.
     * @return A unique pointer to the newly created DescriptorSetLayout.
     *
     * This function finalizes the layout construction and creates a DescriptorSetLayout object.
     * It consumes the builder and returns a unique pointer to the newly created layout.
     */
    std::unique_ptr<DescriptorSetLayout> Build() const;

  private:
    Devices &m_device;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings{};
  };

  /**
   * @brief Constructs a DescriptorSetLayout object.
   * @param device Reference to the Devices object.
   * @param bindings The descriptor bindings for the layout.
   */
  DescriptorSetLayout(
      Devices &device, const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> &bindings);
  ~DescriptorSetLayout();

  // Copy constructors
  DescriptorSetLayout(const DescriptorSetLayout &) = delete;
  DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

  /**
   * @brief Gets the underlying Vulkan descriptor set layout handle.
   * @return The VkDescriptorSetLayout handle.
   */
  VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }

private:
  Devices &m_device;
  VkDescriptorSetLayout m_descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_bindings;

  friend class DescriptorWriter;
};

/**
 * @class DescriptorPool
 * @brief Manages a pool of descriptor sets.
 *
 * This class represents a Vulkan descriptor pool, which is responsible for allocating descriptor sets.
 * Descriptor pools are used to allocate a collection of descriptor sets with specific descriptor types
 * and counts.  Using a pool is more efficient than allocating individual descriptor sets.  This class
 * provides methods for allocating and freeing descriptor sets, as well as resetting the pool.
 */
class BLOOM_API DescriptorPool {
public:
  /**
   * @class Builder
   * @brief Helper class to build DescriptorPool objects.
   *
   *  Simplifies the creation of DescriptorPool objects by allowing you to specify pool sizes,
   *  flags, and the maximum number of sets using a fluent interface.  This builder handles
   *  the details of setting up the VkDescriptorPoolCreateInfo struct.
   */
  class Builder {
  public:
    /**
     * @brief Constructs a Builder object.
     * @param device Reference to the Devices object.  Needed for Vulkan device access.
     */
    explicit Builder(Devices &device) : m_device{device} {}

    /**
     * @brief Adds a descriptor pool size specification.
     * @param descriptorType The type of descriptor (e.g., VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER).
     * @param count The number of descriptors of this type to allocate in the pool.
     * @return Reference to the builder for method chaining.
     *
     *  Specifies the number of descriptors of a particular type to allocate within the pool.
     *  Multiple calls to this function can be made to add different descriptor types to the pool.
     */
    Builder &AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
    /**
     * @brief Sets flags for descriptor pool creation.
     * @param flags Vulkan descriptor pool create flags (e.g., VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT).
     * @return Reference to the builder for method chaining.
     *
     * Allows setting specific flags that control the behavior of the descriptor pool.
     */
    Builder &SetPoolFlags(VkDescriptorPoolCreateFlags flags);
    /**
     * @brief Sets the maximum number of descriptor sets allocatable from the pool.
     * @param count The maximum number of sets.
     * @return Reference to the builder for method chaining.
     *
     *  Defines the upper limit on the number of descriptor sets that can be allocated from this pool.
     */
    Builder &SetMaxSets(uint32_t count);
    /**
     * @brief Builds the DescriptorPool object.
     * @return A unique pointer to the newly created DescriptorPool.
     *
     * Consumes the builder and creates the DescriptorPool object based on the specified parameters.
     */
    std::unique_ptr<DescriptorPool> Build() const;

  private:
    Devices &m_device;
    std::vector<VkDescriptorPoolSize> m_poolSizes{};
    uint32_t m_maxSets = 1000;
    VkDescriptorPoolCreateFlags m_poolFlags = 0;
  };

  /**
   * @brief Constructs a DescriptorPool object.
   * @param device Reference to the Devices object.
   * @param maxSets Maximum number of descriptor sets allocatable from this pool.
   * @param poolFlags Vulkan descriptor pool create flags.
   * @param poolSizes Vector of VkDescriptorPoolSize structs defining the descriptor types and counts.
   */
  DescriptorPool(
      Devices &device,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~DescriptorPool();

  // Copy constructors
  DescriptorPool(const DescriptorPool &) = delete;
  DescriptorPool &operator=(const DescriptorPool &) = delete;

  /**
   * @brief Allocates a descriptor set from the pool.
   * @param descriptorSetLayout The layout of the descriptor set to allocate.
   * @param descriptor Output parameter for the allocated descriptor set handle.
   * @return True if the allocation was successful, false otherwise.
   *
   * Allocates a single descriptor set from the pool based on the provided layout.  Returns
   * false if the pool is full or if there are other allocation errors.
   */
  bool AllocateDescriptor(VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
  /**
   * @brief Frees allocated descriptor sets.
   * @param descriptors Vector of descriptor set handles to free.
   *
   * Returns the given descriptor sets to the pool, making them available for reuse.
   */
  void FreeDescriptors(const std::vector<VkDescriptorSet> &descriptors) const;
  /**
   * @brief Resets the descriptor pool.
   *
   * Resets the descriptor pool, freeing all allocated descriptor sets.  This is useful
   * for reusing the pool instead of creating a new one.
   */
  void ResetPool() const;
  VkDescriptorPool GetVkDescriptorPool() const { return m_descriptorPool; }

private:
  Devices &m_device;
  VkDescriptorPool m_descriptorPool;

  friend class DescriptorWriter;
};

/**
 * @class DescriptorWriter
 * @brief Helper class to write data to descriptor sets.
 *
 * This class simplifies the process of updating descriptor sets with buffer and image information.
 * It provides methods for writing buffer and image data to specific binding points in a descriptor set.
 * The DescriptorWriter works in conjunction with a DescriptorSetLayout and a DescriptorPool to manage
 * the descriptor update process.  It allows batched updates using vkUpdateDescriptorSets for efficiency.
 */
class BLOOM_API DescriptorWriter {
 public:
  /**
   * @brief Constructs a DescriptorWriter.
   * @param setLayout The descriptor set layout.
   * @param pool The descriptor pool.
   */
  DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool);

  /**
   * @brief Writes buffer data to a descriptor set.
   * @param binding The binding point in the shader.
   * @param bufferInfo Pointer to the VkDescriptorBufferInfo containing the buffer data.
   * @return Reference to the writer for chaining.
   *
   * Stages a write operation for buffer data to the specified binding point.  The actual write
   * occurs when Build() or Overwrite() is called.
   */
  DescriptorWriter &WriteBuffer(uint32_t binding, const VkDescriptorBufferInfo *bufferInfo);
  /**
   * @brief Writes image data to a descriptor set.
   * @param binding The binding point in the shader.
   * @param imageInfo Pointer to the VkDescriptorImageInfo containing the image data.
   * @return Reference to the writer for chaining.
   *
   * Stages a write operation for image data to the specified binding point.  The actual write
   * occurs when Build() or Overwrite() is called.
   */
  DescriptorWriter &WriteImage(uint32_t binding, const VkDescriptorImageInfo *imageInfo);
  /**
   * @brief Allocates a descriptor set and writes the staged descriptor data.
   * @param set Output parameter to receive the allocated and updated descriptor set.
   * @return True if successful, false otherwise (e.g., allocation failure).
   *
   * Allocates a descriptor set from the associated DescriptorPool and populates it with
   * the data previously staged using WriteBuffer() and WriteImage().
   */
  bool Build(VkDescriptorSet &set);
  /**
   * @brief Writes the staged descriptor data to an existing descriptor set.
   * @param set The descriptor set to update.
   *
   *  Updates an existing descriptor set with the data previously staged using WriteBuffer()
   *  and WriteImage(). Does not allocate a new descriptor set.
   */
  void Overwrite(const VkDescriptorSet &set);

 private:
  DescriptorSetLayout &m_setLayout;
  DescriptorPool &m_pool;
  std::vector<VkWriteDescriptorSet> m_writes;
};

}
