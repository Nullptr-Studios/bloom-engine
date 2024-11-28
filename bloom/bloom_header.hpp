/**
 * \file bloom_header.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 23/11/2024
 *
 * \brief Precompiled header
 */

#pragma once
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "src/log.hpp"

#ifdef BLOOM_PLATFORM_WINDOWS
#include <Windows.h>
#endif