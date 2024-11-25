/**
 * \file bloom_header.h
 * \author Xein <xgonip@gmail.com>
 * \date 23/11/2024
 *
 * \brief Precompiled header
 */

#pragma once

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

#include "src/log.h"

#ifdef BLOOM_PLATFORM_WINDOWS
#include <Windows.h>
#endif