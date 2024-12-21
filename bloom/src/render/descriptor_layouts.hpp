/**
 * @file descriptor_layouts.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 20/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <bloom_header.hpp>
#include "descriptors.hpp"

namespace bloom::render {

struct BLOOM_API DescriptorLayouts {
  DescriptorSetLayout* globalLayout;
  DescriptorSetLayout* materialLayout;
};

}

