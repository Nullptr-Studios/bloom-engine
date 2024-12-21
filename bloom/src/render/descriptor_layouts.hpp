/**
 * @file descriptor_layouts.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 20/12/2024
 *
 * @brief Contains the DescriptorLayouts struct
 */

#pragma once
#include "descriptors.hpp"

namespace bloom::render {

/**
 * @struct DescriptorLayouts
 * @brief The @c DescriptorLayouts struct contains the descriptor set layouts for the global and material descriptor sets.
 */
struct BLOOM_API DescriptorLayouts {
  DescriptorSetLayout* globalLayout;
  DescriptorSetLayout* materialLayout;
};

}

